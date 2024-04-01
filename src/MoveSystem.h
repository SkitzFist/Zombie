#ifndef ZOMBIE_MOVE_SYSTEM_H_
#define ZOMBIE_MOVE_SYSTEM_H_

#include <smmintrin.h>

#include "MathHack.hpp"
#include "PositionComponent.h"
#include "SpeedComponent.h"
#include "ThreadPool.h"
#include "raylib.h"

// debug
#include "Log.hpp"

inline void moveX(int startIndex, int length, PositionComponent &positions, SpeedComponent &speeds) {
    int i = startIndex;
    for (; i + 3 < (startIndex + length); i += 4) {
        __m128 xPos = _mm_loadu_ps(&positions.xPos[i]);
        __m128 velX = _mm_loadu_ps(&speeds.velX[i]);

        __m128 result = _mm_add_ps(xPos, velX);

        _mm_storeu_ps(&positions.xPos[i], result);
    }

    for (; i < (startIndex + length); ++i) {
        positions.xPos[i] += speeds.velX[i];
    }
}

inline void moveY(int startIndex, int length, PositionComponent &positions, SpeedComponent &speeds) {
    int i = startIndex;
    for (; i + 3 < (startIndex + length); i += 4) {
        __m128 yPos = _mm_loadu_ps(&positions.yPos[i]);
        __m128 velY = _mm_loadu_ps(&speeds.velY[i]);

        __m128 result = _mm_add_ps(yPos, velY);

        _mm_storeu_ps(&positions.yPos[i], result);
    }

    for (; i < (startIndex + length); ++i) {
        positions.yPos[i] += speeds.velY[i];
    }
}

inline constexpr const float MAX_VEL = 5.f;

inline void updateVelX(int startIndex, int length, SpeedComponent &speeds) {
    // Define max and min velocity as __m128 vectors
    __m128 maxVel = _mm_set1_ps(MAX_VEL);
    __m128 minVel = _mm_set1_ps(-MAX_VEL);

    int i = startIndex;
    // Process in chunks of 4 floats since SSE can handle 4 floats at a time
    for (; i + 3 < (startIndex + length); i += 4) {
        // Load 4 velocity and acceleration elements from the arrays
        __m128 velX = _mm_loadu_ps(&speeds.velX[i]);
        __m128 accX = _mm_loadu_ps(&speeds.accX[i]);

        // Add acceleration to velocity
        __m128 updatedVelX = _mm_add_ps(velX, accX);

        // Clamp the updated velocities to the max and min values
        updatedVelX = _mm_max_ps(minVel, _mm_min_ps(maxVel, updatedVelX));

        // Store the clamped velocities back into the array
        _mm_storeu_ps(&speeds.velX[i], updatedVelX);
    }

    // Process any remaining elements that don't fit into a vector of 4
    for (; i < (startIndex + length); ++i) {
        speeds.velX[i] += speeds.accX[i];
        speeds.velX[i] = MathHack::clamp(speeds.velX[i], -MAX_VEL, MAX_VEL);
    }
}

inline void updateVelY(int startIndex, int length, SpeedComponent &speeds) {
    __m128 maxVel = _mm_set1_ps(MAX_VEL);
    __m128 minVel = _mm_set1_ps(-MAX_VEL);

    int i = startIndex;
    for (; i + 3 < (startIndex + length); i += 3) {
        __m128 velY = _mm_loadu_ps(&speeds.velY[i]);
        __m128 accY = _mm_loadu_ps(&speeds.accY[i]);

        __m128 updatedVelY = _mm_add_ps(velY, accY);

        updatedVelY = _mm_max_ps(minVel, _mm_min_ps(maxVel, updatedVelY));

        _mm_storeu_ps(&speeds.velY[i], updatedVelY);
    }
}

struct MoveSystem {
    bool isEnabled = true;

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
    }
};

#endif
