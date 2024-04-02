#ifndef ZOMBIE_MF_BOID_SYSTEM_H_
#define ZOMBIE_MF_BOID_SYSTEM_H_

#include <cmath>
#include <emmintrin.h> // SSE2
#include <smmintrin.h> // SSE4.1

#include "BoidComponent.h"
#include "MathHack.hpp"
#include "PositionComponent.h"
#include "QuadTree.h"
#include "SearchResult.h"
#include "SpeedComponent.h"
#include "ThreadPool.h"
#include "raylib.h"

// debug

#include "Log.hpp"
/*
inline void calcAlignmentAVX2(int id, SearchResult &searchResult, SpeedComponent &speeds, BoidComponent &boids) {
    int count = 0;
    __m256 alignmentX = _mm256_setzero_ps(); // Zero initialize AVX2 register for X
    __m256 alignmentY = _mm256_setzero_ps(); // Zero initialize AVX2 register for Y

    for (int i = 0; i < searchResult.size; i += 8) {
        int blockSize = std::min(8, searchResult.size - i);

        __m256 velX, velY;
        if (blockSize < 8) {
            float tempX[8] = {0};
            float tempY[8] = {0};
            for (int j = 0; j < blockSize; ++j) {
                int otherID = searchResult.arr[i + j];
                if (otherID == id)
                    continue; // Skip if the same ID

                tempX[j] = speeds.velX[otherID];
                tempY[j] = speeds.velY[otherID];
                count++;
            }
            velX = _mm256_loadu_ps(tempX); // Unaligned load for X
            velY = _mm256_loadu_ps(tempY); // Unaligned load for Y
        } else {
            velX = _mm256_set_ps(speeds.velX[searchResult.arr[i + 7]],
                                 speeds.velX[searchResult.arr[i + 6]],
                                 speeds.velX[searchResult.arr[i + 5]],
                                 speeds.velX[searchResult.arr[i + 4]],
                                 speeds.velX[searchResult.arr[i + 3]],
                                 speeds.velX[searchResult.arr[i + 2]],
                                 speeds.velX[searchResult.arr[i + 1]],
                                 speeds.velX[searchResult.arr[i]]);
            velY = _mm256_set_ps(speeds.velY[searchResult.arr[i + 7]],
                                 speeds.velY[searchResult.arr[i + 6]],
                                 speeds.velY[searchResult.arr[i + 5]],
                                 speeds.velY[searchResult.arr[i + 4]],
                                 speeds.velY[searchResult.arr[i + 3]],
                                 speeds.velY[searchResult.arr[i + 2]],
                                 speeds.velY[searchResult.arr[i + 1]],
                                 speeds.velY[searchResult.arr[i]]);
            count += 8;
        }

        alignmentX = _mm256_add_ps(alignmentX, velX);
        alignmentY = _mm256_add_ps(alignmentY, velY);
    }

    // Reduction to scalar
    float finalAlignmentX = _mm256_cvtss_f32(_mm256_hadd_ps(_mm256_hadd_ps(alignmentX, alignmentX), _mm256_setzero_ps()));
    float finalAlignmentY = _mm256_cvtss_f32(_mm256_hadd_ps(_mm256_hadd_ps(alignmentY, alignmentY), _mm256_setzero_ps()));

    if (count > 0) {
        finalAlignmentX -= speeds.velX[id];
        finalAlignmentY -= speeds.velY[id];
        --count;

        finalAlignmentX /= count;
        finalAlignmentY /= count;

        // Adjust based on own velocity
        finalAlignmentX -= speeds.velX[id];
        finalAlignmentY -= speeds.velY[id];
    }

    const float alignmentForce = 1.5f;
    boids.alignemnts[id].x = finalAlignmentX * alignmentForce;
    boids.alignemnts[id].y = finalAlignmentY * alignmentForce;
}
*/

inline void calcAlignmentSSE(int id, SearchResult &searchResult, SpeedComponent &speeds, BoidComponent &boids) {
    __m128 alignmentX = _mm_setzero_ps();
    __m128 alignmentY = _mm_setzero_ps();
    __m128 velX = _mm_setzero_ps();
    __m128 velY = _mm_setzero_ps();

    float finalAlignmentX = -speeds.velX[id];
    float finalAlignmentY = -speeds.velY[id];

    int count = -1;

    int i = 0;
    for (; i + 3 < searchResult.size; i += 4) {
        velX = _mm_set_ps(
            speeds.velX[searchResult.arr[i]],
            speeds.velX[searchResult.arr[i + 1]],
            speeds.velX[searchResult.arr[i + 2]],
            speeds.velX[searchResult.arr[i + 3]]);

        velY = _mm_set_ps(
            speeds.velY[searchResult.arr[i]],
            speeds.velY[searchResult.arr[i + 1]],
            speeds.velY[searchResult.arr[i + 2]],
            speeds.velY[searchResult.arr[i + 3]]);

        alignmentX = _mm_add_ps(alignmentX, velX);
        alignmentY = _mm_add_ps(alignmentY, velY);

        count += 4;
    }

    alignmentX = _mm_hadd_ps(alignmentX, alignmentX);
    alignmentX = _mm_hadd_ps(alignmentX, alignmentX);
    finalAlignmentX = _mm_cvtss_f32(alignmentX);

    alignmentY = _mm_hadd_ps(alignmentY, alignmentY);
    alignmentY = _mm_hadd_ps(alignmentY, alignmentY);
    finalAlignmentY = _mm_cvtss_f32(alignmentY);

    for (; i < searchResult.size; ++i) {
        finalAlignmentX += speeds.velX[searchResult.arr[i]];
        finalAlignmentY += speeds.velY[searchResult.arr[i]];
        ++count;
    }

    finalAlignmentX /= count;
    finalAlignmentY /= count;

    const float alignmentForce = 0.75f;
    finalAlignmentX *= alignmentForce;
    finalAlignmentY *= alignmentForce;

    boids.alignments[id].x = finalAlignmentX;
    boids.alignments[id].y = finalAlignmentY;
}

inline void calcAlignment(int id, SearchResult &searchResult, SpeedComponent &speeds, BoidComponent &boids) {
    int count = 0;
    Vector2 alignment = {0.f, 0.f};
    for (int i = 0; i < searchResult.size; ++i) {
        int otherID = searchResult.arr[i];
        if (otherID == id) {
            continue;
        }

        alignment.x += speeds.velX[otherID];
        alignment.y += speeds.velY[otherID];
        ++count;
    }

    alignment.x /= count;
    alignment.y /= count;

    const float alignmentForce = 0.75f;

    alignment.x *= alignmentForce;
    alignment.y *= alignmentForce;
    boids.alignments[id] = alignment;
}

inline static bool useSSE = false;

inline void calculateAlignments(SearchResult &alignSearch,
                                QuadTree &quadTree,
                                PositionComponent &positions,
                                SpeedComponent &speeds,
                                BoidComponent &boids,
                                int maxEntities,
                                float entityRadius,
                                int startIndex,
                                int length) {

    Rectangle alignArea;
    float alignSize = 15.f;
    alignArea.width = entityRadius * alignSize;
    alignArea.height = entityRadius * alignSize;
    for (int i = startIndex; i < (startIndex + length); ++i) {
        alignArea.x = positions.xPos[i] - ((alignArea.width / 2.f) - entityRadius);
        alignArea.y = positions.yPos[i] - ((alignArea.height / 2.f) - entityRadius);
        alignSearch.clear();
        quadTree.search(alignArea, positions, alignSearch, entityRadius);

        if (alignSearch.size > 1) {
            if (useSSE) {
                calcAlignmentSSE(i, alignSearch, speeds, boids);
            } else {
                calcAlignment(i, alignSearch, speeds, boids);
            }
        }
    }
}

inline void calcSeparationSSE41(int id, SearchResult &searchResult, PositionComponent &positions, BoidComponent &boids) {
    __m128 separationX = _mm_setzero_ps();
    __m128 separationY = _mm_setzero_ps();
    int count = 0;

    for (int i = 0; i < searchResult.size; i += 4) {
        float xPos[4], yPos[4];
        for (int j = 0; j < 4; ++j) {
            int idx = i + j;
            if (idx < searchResult.size) {
                xPos[j] = positions.xPos[searchResult.arr[idx]];
                yPos[j] = positions.yPos[searchResult.arr[idx]];
            } else {
                xPos[j] = positions.xPos[id];
                yPos[j] = positions.yPos[id];
            }
        }

        __m128 otherPosX = _mm_loadu_ps(xPos);
        __m128 otherPosY = _mm_loadu_ps(yPos);
        __m128 idPosX = _mm_set1_ps(positions.xPos[id]);
        __m128 idPosY = _mm_set1_ps(positions.yPos[id]);

        __m128 diffX = _mm_sub_ps(idPosX, otherPosX);
        __m128 diffY = _mm_sub_ps(idPosY, otherPosY);
        __m128 distSq = _mm_add_ps(_mm_mul_ps(diffX, diffX), _mm_mul_ps(diffY, diffY));
        __m128 invDistSq = _mm_div_ps(_mm_set1_ps(1.0f), distSq);

        diffX = _mm_mul_ps(diffX, invDistSq);
        diffY = _mm_mul_ps(diffY, invDistSq);

        separationX = _mm_add_ps(separationX, diffX);
        separationY = _mm_add_ps(separationY, diffY);

        count += (searchResult.size - i) >= 4 ? 4 : (searchResult.size % 4);
    }

    separationX = _mm_hadd_ps(separationX, separationX);
    separationX = _mm_hadd_ps(separationX, separationX);
    separationY = _mm_hadd_ps(separationY, separationY);
    separationY = _mm_hadd_ps(separationY, separationY);

    float finalSeparationX, finalSeparationY;
    _mm_store_ss(&finalSeparationX, separationX);
    _mm_store_ss(&finalSeparationY, separationY);

    if (count > 0) {
        finalSeparationX /= count;
        finalSeparationY /= count;
    }

    const float separationForce = 10.0f;
    boids.separations[id].x = finalSeparationX * separationForce;
    boids.separations[id].y = finalSeparationY * separationForce;
}

inline void calcSeparation(int id, SearchResult &searchResult, PositionComponent &positions, BoidComponent &boids) {
    int count = 0;
    Vector2 seperation = {0.f, 0.f};
    for (int i = 0; i < searchResult.size; ++i) {
        int otherId = searchResult.arr[i];
        if (otherId == id) {
            continue;
        }

        float distance = MathHack::calculateDistance(
            positions.xPos[id], positions.yPos[id],
            positions.xPos[otherId], positions.yPos[otherId]);

        Vector2 diff = {
            positions.xPos[id] - positions.xPos[otherId],
            positions.yPos[id] - positions.yPos[otherId]};

        float d2 = distance * distance;

        diff.x /= d2;
        diff.y /= d2;
        seperation.x += diff.x;
        seperation.y += diff.y;
        ++count;
    }

    seperation.x /= count;
    seperation.y /= count;

    const float seperationForce = 10.f;

    seperation.x *= seperationForce;
    seperation.y *= seperationForce;

    boids.separations[id].x = seperation.x;
    boids.separations[id].y = seperation.y;
}

inline void calcSeparations(SearchResult &seperationSearch,
                            QuadTree &quadTree,
                            PositionComponent &positions,
                            BoidComponent &boids,
                            int maxEntities,
                            int entityRadius,
                            int startIndex,
                            int length) {
    Rectangle separationArea;
    float seperationSize = 28.f;
    separationArea.width = entityRadius * seperationSize;
    separationArea.height = entityRadius * seperationSize;

    for (int i = startIndex; i < (startIndex + length); ++i) {
        separationArea.x = positions.xPos[i] - ((separationArea.width / 2.f) - entityRadius);
        separationArea.y = positions.yPos[i] - ((separationArea.height / 2.f) - entityRadius);
        seperationSearch.clear();
        quadTree.search(separationArea, positions, seperationSearch, entityRadius);

        if (seperationSearch.size > 1) {
            calcSeparation(i, seperationSearch, positions, boids);
        }
    }
}

inline void calcCohesionSSE(int id, SearchResult &searchResult, PositionComponent &positions, BoidComponent &boids) {
    __m128 cohesionX = _mm_setzero_ps();
    __m128 cohesionY = _mm_setzero_ps();
    __m128 posX = _mm_setzero_ps();
    __m128 posY = _mm_setzero_ps();

    float finalCohesionX = -positions.xPos[id];
    float finalCohesionY = -positions.yPos[id];

    int count = 0;

    int i = 0;
    for (; i + 3 < searchResult.size; i += 4) {
        posX = _mm_set_ps(
            positions.xPos[searchResult.arr[i]],
            positions.xPos[searchResult.arr[i + 1]],
            positions.xPos[searchResult.arr[i + 2]],
            positions.xPos[searchResult.arr[i + 3]]);

        posY = _mm_set_ps(
            positions.yPos[searchResult.arr[i]],
            positions.yPos[searchResult.arr[i + 1]],
            positions.yPos[searchResult.arr[i + 2]],
            positions.yPos[searchResult.arr[i + 3]]);

        cohesionX = _mm_add_ps(cohesionX, posX);
        cohesionY = _mm_add_ps(cohesionY, posY);

        count += 4;
    }

    cohesionX = _mm_hadd_ps(cohesionX, cohesionX);
    cohesionX = _mm_hadd_ps(cohesionX, cohesionX);
    finalCohesionX = _mm_cvtss_f32(cohesionX);

    cohesionY = _mm_hadd_ps(cohesionY, cohesionY);
    cohesionY = _mm_hadd_ps(cohesionY, cohesionY);
    finalCohesionY = _mm_cvtss_f32(cohesionY);

    for (; i < searchResult.size; ++i) {
        finalCohesionX += positions.xPos[searchResult.arr[i]];
        finalCohesionY += positions.yPos[searchResult.arr[i]];
        ++count;
    }

    finalCohesionX /= count;
    finalCohesionY /= count;

    finalCohesionX -= positions.xPos[id];
    finalCohesionY -= positions.yPos[id];

    const float cohesionForce = 0.0060f;
    finalCohesionX *= cohesionForce;
    finalCohesionY *= cohesionForce;

    boids.cohesions[id].x = finalCohesionX;
    boids.cohesions[id].y = finalCohesionY;
}

inline void calcCohesion(int id, SearchResult &searchResult, PositionComponent &positions, BoidComponent &boids) {
    Vector2 cohesion = {0.f, 0.f};
    int count = 0;

    for (int i = 0; i < searchResult.size; ++i) {
        int otherID = searchResult.arr[i];
        if (otherID == id) {
            continue;
        }

        cohesion.x += positions.xPos[otherID];
        cohesion.y += positions.yPos[otherID];
        ++count;
    }

    cohesion.x /= count;
    cohesion.y /= count;

    cohesion.x -= positions.xPos[id];
    cohesion.y -= positions.yPos[id];

    const float cohesionForce = 0.0060f;
    cohesion.x *= cohesionForce;
    cohesion.y *= cohesionForce;

    boids.cohesions[id] = cohesion;
}

inline void calcCohesions(
    SearchResult &cohesionSearch,
    QuadTree &quadTree,
    PositionComponent &positions,
    BoidComponent &boids,
    int maxEntities,
    int entityRadius,
    int startIndex,
    int length) {

    Rectangle cohesionArea;
    float cohesionSize = 40.f;
    cohesionArea.width = entityRadius * cohesionSize;
    cohesionArea.height = entityRadius * cohesionSize;

    for (int i = startIndex; i < (startIndex + length); ++i) {
        cohesionArea.x = positions.xPos[i] - ((cohesionArea.width / 2.f) - entityRadius);
        cohesionArea.y = positions.yPos[i] - ((cohesionArea.height / 2.f) - entityRadius);

        cohesionSearch.clear();
        quadTree.search(cohesionArea, positions, cohesionSearch, entityRadius);

        if (cohesionSearch.size > 1) {
            if (useSSE) {
                calcCohesionSSE(i, cohesionSearch, positions, boids);
            } else {
                calcCohesion(i, cohesionSearch, positions, boids);
            }
        }
    }
}

inline void addForces(int startIndex, int length, SpeedComponent &speeds, BoidComponent &boids) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        Vector2 alignment = boids.alignments[i];
        Vector2 seperation = boids.separations[i];
        Vector2 cohesion = boids.cohesions[i];

        Vector2 acceleration = {
            alignment.x + seperation.x + cohesion.x,
            alignment.y + seperation.y + cohesion.y};

        float maxAcc = 5.f;
        acceleration.x = MathHack::clamp(acceleration.x, -maxAcc, maxAcc);
        acceleration.y = MathHack::clamp(acceleration.y, -maxAcc, maxAcc);

        if ((acceleration.x > 0.f || acceleration.y < 0.f) && (acceleration.y > 0.f || acceleration.y < 0.f)) {
            float length = sqrtf(acceleration.x * acceleration.x) + (acceleration.y * acceleration.y);
            acceleration.x /= length;
            acceleration.y /= length;
        }

        speeds.accX[i] = acceleration.x;
        speeds.accY[i] = acceleration.y;
    }
}

struct MfBoidSystem {
    bool isEnabled = false;
    bool alignmentEnabled = false;
    bool separationEnabled = false;
    bool cohesionEnabled = false;

    SearchResult alignSearch[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    SearchResult separationSearch[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    SearchResult cohesionSearch[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};

    int currentIndex = 0;

    void update(Settings &settings,
                ThreadPool &threadPool,
                QuadTree &tree,
                PositionComponent &positions,
                SpeedComponent &speeds,
                BoidComponent &boids) {

        if (!isEnabled) {
            return;
        }

        int batchSize = settings.big ? settings.MAX_ENTITIES / 30 : settings.MAX_ENTITIES / 1;

        int batchEnd = currentIndex + batchSize;
        if (batchEnd > settings.MAX_ENTITIES) {
            batchEnd = settings.MAX_ENTITIES;
        }

        int numberOfThreads = 8;
        int totalBatchSize = batchEnd - currentIndex;
        int lengthPerThread = totalBatchSize / numberOfThreads;
        int extra = totalBatchSize % numberOfThreads;

        if (separationEnabled) {
            int startIndex = 0;
            int length = 0;
            for (int i = 0; i < numberOfThreads; ++i) {
                startIndex = currentIndex + i * lengthPerThread;
                length = (i < numberOfThreads - 1) ? lengthPerThread : lengthPerThread + extra;
                threadPool.enqueue(calcSeparations,
                                   std::ref(separationSearch[i]),
                                   std::ref(tree),
                                   std::ref(positions),
                                   std::ref(boids),
                                   settings.MAX_ENTITIES,
                                   settings.ZOMBIE_RADIUS,
                                   startIndex,
                                   length);
            }
        }

        if (alignmentEnabled) {
            int startIndex = 0;
            int length = 0;
            for (int i = 0; i < numberOfThreads; ++i) {
                startIndex = currentIndex + (i * lengthPerThread);
                length = (i < numberOfThreads - 1) ? lengthPerThread : lengthPerThread + extra;

                threadPool.enqueue(calculateAlignments,
                                   std::ref(alignSearch[i]),
                                   std::ref(tree),
                                   std::ref(positions),
                                   std::ref(speeds),
                                   std::ref(boids),
                                   settings.MAX_ENTITIES,
                                   settings.ZOMBIE_RADIUS,
                                   startIndex,
                                   length);
            }
        }

        if (cohesionEnabled) {
            int startIndex = 0;
            int length = 0;
            for (int i = 0; i < numberOfThreads; ++i) {
                startIndex = currentIndex + i * lengthPerThread;
                length = (i < numberOfThreads - 1) ? lengthPerThread : lengthPerThread + extra;
                threadPool.enqueue(calcCohesions,
                                   std::ref(cohesionSearch[i]),
                                   std::ref(tree),
                                   std::ref(positions),
                                   std::ref(boids),
                                   settings.MAX_ENTITIES,
                                   settings.ZOMBIE_RADIUS,
                                   startIndex,
                                   length);
            }
        }

        threadPool.awaitCompletion();

        for (int i = 0; i < numberOfThreads; ++i) {
            int startIndex = currentIndex + i * lengthPerThread;
            int length = (i < numberOfThreads - 1) ? lengthPerThread : lengthPerThread + extra;

            threadPool.enqueue(addForces, startIndex, length, std::ref(speeds), std::ref(boids));
        }

        threadPool.awaitCompletion();

        currentIndex += totalBatchSize;

        if (currentIndex >= settings.MAX_ENTITIES) {
            currentIndex = 0;
        }
    }
};

#endif
