#ifndef ZOMBIE_POSITION_COMPONENT_H_
#define ZOMBIE_POSITION_COMPONENT_H_

#include <vector>

#include "raylib.h"
#include "IndexIdLookup.h"

struct PositionComponent{

    PositionComponent(){
        xPos.reserve(10000000);
        yPos.reserve(10000000);
    }

    void add(const float x, const float y, int id){
        (void)id;
        //int index = getFirstFreeIndex(idToIndexLookup);
        ++size;
        //int index = xPos.size();
        xPos.push_back(x);
        yPos.push_back(y);
    }

    void update(const float x, const float y, int index){
        xPos[index] = x;
        yPos[index] = y;
    }

    void remove(int id){
        if(removeId(idToIndexLookup, id)){
            --size;   
        }
    }

    Vector2 getPosition(int id) const{
        int index = getIndex(idToIndexLookup, id);

        return {xPos[index], yPos[index]};
    }

    Vector2 getPositionByIndex(int index) const{
        return {xPos[index], yPos[index]};
    }

    bool exists(int id) const{
        return idExists(idToIndexLookup , id);
    }

    std::vector<float> xPos;
    std::vector<float> yPos;
    int size = 0;
    std::vector<IndexId> idToIndexLookup;
};

#endif //ZOMBIE_POSITION_COMPONENT_H_
