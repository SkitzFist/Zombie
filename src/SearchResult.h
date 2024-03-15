#ifndef ZOMBIE_SEARCH_RESULT_H_
#define ZOMBIE_SEARCH_RESULT_H_

struct SearchResult {
    int size;
    int *arr;

    SearchResult(int maxSize) : size(0) {
        arr = new int[maxSize];
    }

    ~SearchResult(){
        delete[] arr;
    }

    void add(int entityID) {
        arr[size++] = entityID;
    }

    void clear() {
        size = 0;
    }
};

#endif