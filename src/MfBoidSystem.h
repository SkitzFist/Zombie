#ifndef ZOMBIE_MF_BOID_SYSTEM_H_
#define ZOMBIE_MF_BOID_SYSTEM_H_

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

    if (count > 0) {
        alignment.x /= count;
        alignment.y /= count;

        alignment.x -= speeds.velX[id];
        alignment.y -= speeds.velY[id];
    }
    const float alignmentForce = 0.75f;

    alignment.x *= alignmentForce;
    alignment.y *= alignmentForce;
    boids.alignemnts[id] = alignment;
}

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

        calcAlignment(i, alignSearch, speeds, boids);
    }
}

inline void calcSeperation(int id, SearchResult &searchResult, PositionComponent &positions, BoidComponent &boids) {
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

    if (count > 0) {
        seperation.x /= count;
        seperation.y /= count;
    }

    const float seperationForce = 5.f;

    seperation.x *= seperationForce;
    seperation.y *= seperationForce;

    boids.seperations[id].x = seperation.x;
    boids.seperations[id].y = seperation.y;
}

inline void calcSeperations(SearchResult &seperationSearch,
                            QuadTree &quadTree,
                            PositionComponent &positions,
                            BoidComponent &boids,
                            int maxEntities,
                            int entityRadius,
                            int startIndex,
                            int length) {
    Rectangle separationArea;
    float seperationSize = 14.f;
    separationArea.width = entityRadius * seperationSize;
    separationArea.height = entityRadius * seperationSize;

    for (int i = startIndex; i < (startIndex + length); ++i) {
        separationArea.x = positions.xPos[i] - ((separationArea.width / 2.f) - entityRadius);
        separationArea.y = positions.yPos[i] - ((separationArea.height / 2.f) - entityRadius);
        seperationSearch.clear();
        quadTree.search(separationArea, positions, seperationSearch, entityRadius);

        calcSeperation(i, seperationSearch, positions, boids);
    }
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

    if (count > 0) {
        cohesion.x /= count;
        cohesion.y /= count;

        cohesion.x -= positions.xPos[id];
        cohesion.y -= positions.yPos[id];
    }

    const float cohesionForce = 0.0015f;
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
    float cohesionSize = 20.f;
    cohesionArea.width = entityRadius * cohesionSize;
    cohesionArea.height = entityRadius * cohesionSize;

    for (int i = startIndex; i < (startIndex + length); ++i) {
        cohesionArea.x = positions.xPos[i] - ((cohesionArea.width / 2.f) - entityRadius);
        cohesionArea.y = positions.yPos[i] - ((cohesionArea.height / 2.f) - entityRadius);
        cohesionSearch.clear();
        quadTree.search(cohesionArea, positions, cohesionSearch, entityRadius);
        calcCohesion(i, cohesionSearch, positions, boids);
    }
}

inline void addForces(int startIndex, int length, SpeedComponent &speeds, BoidComponent &boids) {
    for (int i = startIndex; i < (startIndex + length); ++i) {
        Vector2 alignment = boids.alignemnts[i];
        Vector2 seperation = boids.seperations[i];
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
    bool isEnabled = true;
    bool alignemntEnabled = false;
    bool seperationEnabled = false;
    bool cohesionEnabled = false;

    SearchResult alignSearch[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    SearchResult seperationSearch[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
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

        if (currentIndex == settings.MAX_ENTITIES) {
            currentIndex = 0;
        }

        int batchSize = settings.MAX_ENTITIES / 4;
        int batch = currentIndex + batchSize;

        if (batch >= settings.MAX_ENTITIES) {
            batch = settings.MAX_ENTITIES;
        }

        int numberOfThreads = 8;
        int length = batchSize / numberOfThreads;
        int startIndex = currentIndex;

        currentIndex += batchSize;

        if (seperationEnabled) {
            for (int i = 0; i < numberOfThreads; ++i) {
                threadPool.enqueue(calcSeperations,
                                   std::ref(seperationSearch[i]),
                                   std::ref(tree),
                                   std::ref(positions),
                                   std::ref(boids),
                                   settings.MAX_ENTITIES,
                                   settings.ZOMBIE_RADIUS,
                                   startIndex,
                                   length);
                startIndex += length;
            }
        }

        startIndex = 0;
        if (alignemntEnabled) {
            for (int i = 0; i < numberOfThreads; ++i) {
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
                startIndex += length;
            }
        }

        startIndex = 0;
        if (cohesionEnabled) {
            for (int i = 0; i < numberOfThreads; ++i) {
                threadPool.enqueue(calcCohesions,
                                   std::ref(cohesionSearch[i]),
                                   std::ref(tree),
                                   std::ref(positions),
                                   std::ref(boids),
                                   settings.MAX_ENTITIES,
                                   settings.ZOMBIE_RADIUS,
                                   startIndex,
                                   length);
                startIndex += length;
            }
        }

        threadPool.awaitCompletion();

        startIndex = 0;
        for (int i = 0; i < numberOfThreads; ++i) {
            threadPool.enqueue(addForces, startIndex, length, std::ref(speeds), std::ref(boids));
            startIndex += length;
        }

        // threadPool.awaitCompletion();
    }
};

#endif
