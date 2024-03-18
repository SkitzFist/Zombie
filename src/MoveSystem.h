#ifndef ZOMBIE_MOVE_SYSTEM_H_
#define ZOMBIE_MOVE_SYSTEM_H_

#include "raylib.h"
#include "ThreadPool.h"
#include "PositionComponent.h"

inline void move(int startIndex, int length, PositionComponent &positions, const float dt) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
          Vector2 pos = {positions.xPos[i], 0.f};
          pos.x += 150.f * dt;
          positions.xPos[i] = pos.x;          
    }
}

struct MoveSystem {
    bool isActive = false;

    void update(PositionComponent &positions, ThreadPool& threadPool) {
        if (!isActive) {
            return;
        }

        int numberOfThreads = 4;
        int length = positions.size / numberOfThreads;
        int startIndex = 0;

        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(move, startIndex, length, std::ref(positions), GetFrameTime());
            startIndex += length;
        }
    }
};

#endif
