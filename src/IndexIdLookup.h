#ifndef ZOMBIE_INDEX_ID_LOOKUP_H_
#define ZOMBIE_INDEX_ID_LOOKUP_H_

#include <vector>

struct IndexId{
    int id;
    int index;
    bool removed;

    IndexId(int id, int index, bool removed):id(id), index(index), removed(removed){}
};

inline bool idExists(const std::vector<IndexId>& vec, int id){

    for(const IndexId& ii : vec){
        if(ii.id == id && !ii.removed){
            return true;
        }
    }

    return false;
}

inline int getFirstFreeIndex(std::vector<IndexId>& vec){
    for(IndexId& ii : vec){
        if(ii.removed){
            ii.removed = false;
            return ii.index;
        }
    }

    return -1;
}

inline bool removeId( std::vector<IndexId>& vec , int id ){
    for(IndexId& ii : vec){
        if(ii.id == id){
            ii.removed = true;
            return true;
        }
    }
    return false;
}

inline int getIndex(const std::vector<IndexId>& vec, int id){
    for(const IndexId& ii : vec){
        if(ii.id == id){
            return ii.index;
        }
    }
    return -1;
}

#endif
