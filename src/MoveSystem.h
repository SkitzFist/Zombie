#ifndef ZOMBIE_MOVE_SYSTEM_H_
#define ZOMBIE_MOVE_SYSTEM_H_

#include "PositionComponent.h"
#include "SpeedComponent.h"
#include "ThreadPool.h"
#include "raylib.h"

// debug
#include "Log.hpp"

inline void moveX(int startIndex, int length, PositionComponent &positions, SpeedComponent &speeds) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        positions.xPos[i] += speeds.velX[i];
        positions.yPos[i] += speeds.velY[i];
    }
}

inline void moveY(int startIndex, int length, PositionComponent &positions, SpeedComponent &speeds) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        positions.yPos[i] += speeds.velY[i];
    }
}

inline void updateVelX(int startIndex, int length, SpeedComponent &speeds) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        speeds.velX[i] += speeds.accX[i];
        speeds.velY[i] += speeds.accY[i];
    }
}

inline void updateVelY(int startIndex, int length, SpeedComponent &speeds) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        speeds.velY[i] += speeds.accY[i];
    }
}

struct MoveSystem {
    bool isEnabled = false;

    void update(PositionComponent &positions, SpeedComponent &speeds, ThreadPool &threadPool) {
        if (!isEnabled) {
            return;
        }


        int numberOfThreads = 4;
        int length = positions.size / numberOfThreads;
        int startIndex = 0;

        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(updateVelX, startIndex, length, std::ref(speeds));
            startIndex += length;
        }

        startIndex = 0;
        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(moveX, startIndex, length, std::ref(positions), std::ref(speeds));
            startIndex += length;
        }
        /*
        startIndex = 0;
        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(updateVelY, startIndex, length, std::ref(speeds));
            startIndex += length;
        }

        startIndex = 0;
        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(moveY, startIndex, length, std::ref(positions), std::ref(speeds));
            startIndex += length;
        }
        */
    }
};

#endif
