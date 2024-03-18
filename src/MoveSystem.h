#ifndef ZOMBIE_MOVE_SYSTEM_H_
#define ZOMBIE_MOVE_SYSTEM_H_

#include "raylib.h"
#include "ThreadPool.h"
#include "PositionComponent.h"
#include "SpeedComponent.h"

inline void move(int startIndex, int length, PositionComponent &positions, SpeedComponent& speeds,const float dt) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
          int entityId = i;

          speeds.velX[entityId] += speeds.accX[entityId];
          speeds.velY[entityId] += speeds.accY[entityId];

          positions.xPos[entityId] += speeds.velX[entityId];
          positions.yPos[entityId] += speeds.velY[entityId];
    }
}

struct MoveSystem {
    bool isActive = false;

    void update(PositionComponent &positions, SpeedComponent& speeds, ThreadPool& threadPool) {
        if (!isActive) {
            return;
        }

        int numberOfThreads = 4;
        int length = positions.size / numberOfThreads;
        int startIndex = 0;

        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(move, startIndex, length, std::ref(positions), std::ref(speeds), GetFrameTime());
            startIndex += length;
        }
    }
};

#endif
