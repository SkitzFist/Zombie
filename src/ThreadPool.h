#ifndef ZOMBIE_THREAD_POOL_H_
#define ZOMBIE_THREAD_POOL_H_

#include <functional>
#include <future>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>

class ThreadPool {
  public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

    void awaitCompletion();

  private:
    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;
    size_t tasks_in_progress = 0;

    pthread_mutex_t queue_mutex;
    pthread_cond_t condition;
    pthread_cond_t completion_condition;
    bool stop;

    static void *worker(void *arg);
};

inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
    pthread_cond_init(&completion_condition, nullptr);

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
    pthread_cond_destroy(&completion_condition);
}

template <class F, class... Args>
inline auto ThreadPool::enqueue(F &&f, Args &&...args)->std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    pthread_mutex_lock(&queue_mutex);
    if (stop) {
        pthread_mutex_unlock(&queue_mutex);
        throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.emplace([task]() { (*task)(); });
    tasks_in_progress++;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&queue_mutex);

    return res;
}

inline void ThreadPool::awaitCompletion() {
    pthread_mutex_lock(&queue_mutex);
    while (!tasks.empty() || tasks_in_progress > 0) {
        pthread_cond_wait(&completion_condition, &queue_mutex);
    }
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
        pthread_mutex_lock(&pool->queue_mutex);
        pool->tasks_in_progress--;
        if (pool->tasks_in_progress == 0 && pool->tasks.empty()) {
            pthread_cond_signal(&pool->completion_condition);
        }
        pthread_mutex_unlock(&pool->queue_mutex);
    }
    return nullptr;
}

#endif
