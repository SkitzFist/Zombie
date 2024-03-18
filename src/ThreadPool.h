#ifndef ZOMBIE_THREAD_POOL_H_
#define ZOMBIE_THREAD_POOL_H_

#include <functional>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <future>

class ThreadPool {
  public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template <class F, class... Args>
    void enqueue(F &&f, Args &&...args);

  private:
    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;

    pthread_mutex_t queue_mutex;
    pthread_cond_t condition;
    bool stop;

    static void *worker(void *arg);
};

inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&condition, nullptr);

    for (size_t i = 0; i < threads; ++i) {
        pthread_t worker_thread;
        if (pthread_create(&worker_thread, nullptr, ThreadPool::worker, this) != 0) {
            perror("pthread_create");
            throw std::runtime_error("Error creating thread");
        }
        workers.push_back(worker_thread);
    }
}

inline ThreadPool::~ThreadPool() {
    {
        pthread_mutex_lock(&queue_mutex);
        stop = true;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&queue_mutex);
    }

    for (auto &worker : workers) {
        pthread_join(worker, nullptr);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&condition);
}

template <class F, class... Args>
inline void ThreadPool::enqueue(F &&f, Args &&...args) {
    auto task = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    pthread_mutex_lock(&queue_mutex);
    if (stop) {
        pthread_mutex_unlock(&queue_mutex);
        throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.emplace([task]() { (*task)(); });
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&queue_mutex);
}

inline void *ThreadPool::worker(void *arg) {
    auto *pool = static_cast<ThreadPool *>(arg);
    for (;;) {
        std::function<void()> task;

        pthread_mutex_lock(&pool->queue_mutex);
        while (!pool->stop && pool->tasks.empty()) {
            pthread_cond_wait(&pool->condition, &pool->queue_mutex);
        }
        if (pool->stop && pool->tasks.empty()) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }
        task = std::move(pool->tasks.front());
        pool->tasks.pop();
        pthread_mutex_unlock(&pool->queue_mutex);

        task();
    }
    return nullptr;
}

#endif
