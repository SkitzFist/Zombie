#ifndef ZOMBIE_SPEED_COMPONENT_H_
#define ZOMBIE_SPEED_COMPONENT_H_

#include <vector>

#include "raylib.h"
#include "Settings.h"

struct SpeedComponent{

    const float maxSpeed = 25.f;

    std::vector<float> velX;
    std::vector<float> velY;
    std::vector<float> accX;
    std::vector<float> accY;

    int size = 0;

    SpeedComponent(const Settings& settings){
      velX.reserve(settings.MAX_ENTITIES);
      velY.reserve(settings.MAX_ENTITIES);

      accX.reserve(settings.MAX_ENTITIES);
      accY.reserve(settings.MAX_ENTITIES);
    }

    void add(const float vx, const float vy, const float ax, const float ay){
      velX.push_back(vx);
      velY.push_back(vy);

      accX.push_back(ax);
      accY.push_back(ay);
    }

};

#endif
