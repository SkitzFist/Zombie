#ifndef ZOMBIE_SEARCH_RESULT_H_
#define ZOMBIE_SEARCH_RESULT_H_

#include "Log.hpp"

struct SearchResult {
    int size;
    int *arr;
    const int MAX_SIZE;
    SearchResult(int maxSize) : size(0), MAX_SIZE(maxSize) {
        arr = new int[maxSize];
    }

    ~SearchResult(){
        delete[] arr;
    }

    void add(int entityID) {
        if(size == MAX_SIZE){
            Log::info("SEARCH RESULT : MAX SIZE REACHED, DO SOMETHING ABOUT IT NOW!");
        }
        arr[size++] = entityID;
    }

    void clear() {
        size = 0;
    }
};

#endif