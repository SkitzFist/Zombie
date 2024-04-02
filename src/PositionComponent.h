#ifndef ZOMBIE_POSITION_COMPONENT_H_
#define ZOMBIE_POSITION_COMPONENT_H_

#include <vector>

#include "raylib.h"
#include "Settings.h"

struct PositionComponent{

    PositionComponent(const Settings& settings){
        xPos.reserve(settings.MAX_ENTITIES);
        yPos.reserve(settings.MAX_ENTITIES);
    }

    void add(const float x, const float y, int id){
        (void)id;
        ++size;
        xPos.push_back(x);
        yPos.push_back(y);
    }

    void update(const float x, const float y, int index){
        xPos[index] = x;
        yPos[index] = y;
    }

    std::vector<float> xPos;
    std::vector<float> yPos;
    int size = 0;
};

#endif //ZOMBIE_POSITION_COMPONENT_H_
