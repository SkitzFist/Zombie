#ifndef ZOMBIE_BOID_SYSTEM_H_
#define ZOMBIE_BOID_SYSTEM_H_

#include "MathHack.hpp"
#include "PositionComponent.h"
#include "QuadTree.h"
#include "SearchResult.h"
#include "Settings.h"
#include "SpeedComponent.h"

inline Vector2 calcAlign(int id, SearchResult &searchResult, SpeedComponent &speeds) {
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

    return alignment;
}

inline Vector2 calcSeperation(int id, SearchResult &searchResult, PositionComponent &positions) {
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

    return seperation;
}

inline Vector2 calcCohesion(int id, SearchResult &searchResult, PositionComponent &positions) {
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

    return cohesion;
}

struct BoidSystem {
    bool isEnabled = true;
    bool alignemntEnabled = false;
    bool seperationEnabled = false;
    bool cohesionEnabled = false;

    SearchResult alignSearch;
    SearchResult seperationSearch;
    SearchResult cohesionSearch;

    BoidSystem(Settings &settings) : alignSearch(settings.MAX_ENTITIES),
                                     seperationSearch(settings.MAX_ENTITIES),
                                     cohesionSearch(settings.MAX_ENTITIES) {
    }

    void update(Settings &settings, QuadTree &quadTree, SpeedComponent &speeds, PositionComponent &positions) {
        if (!isEnabled) {
            return;
        }

        Rectangle alignArea;
        float alignSize = 15.f;
        alignArea.width = settings.ZOMBIE_RADIUS * alignSize;
        alignArea.height = settings.ZOMBIE_RADIUS * alignSize;

        Rectangle separationArea;
        float seperationSize = 14.f;
        separationArea.width = settings.ZOMBIE_RADIUS * seperationSize;
        separationArea.height = settings.ZOMBIE_RADIUS * seperationSize;

        Rectangle cohesionArea;
        float cohesionSize = 20.f;
        cohesionArea.width = settings.ZOMBIE_RADIUS * cohesionSize;
        cohesionArea.height = settings.ZOMBIE_RADIUS * cohesionSize;

        for (int i = 0; i < settings.MAX_ENTITIES; ++i) {
            
            Vector2 alignment = {0.f, 0.f};
            if (alignemntEnabled) {
                alignArea.x = positions.xPos[i] - ((alignArea.width / 2.f) - settings.ZOMBIE_RADIUS);
                alignArea.y = positions.yPos[i] - ((alignArea.height / 2.f) - settings.ZOMBIE_RADIUS);
                alignSearch.clear();
                quadTree.search(alignArea, positions, alignSearch, settings.ZOMBIE_RADIUS);
                alignment = calcAlign(i, alignSearch, speeds);
            }

            const float alignmentForce = 0.75f;

            alignment.x *= alignmentForce;
            alignment.y *= alignmentForce;

            Vector2 seperation = {0.f, 0.f};
            if (seperationEnabled) {
                separationArea.x = positions.xPos[i] - ((separationArea.width / 2.f) - settings.ZOMBIE_RADIUS);
                separationArea.y = positions.yPos[i] - ((separationArea.height / 2.f) - settings.ZOMBIE_RADIUS);
                seperationSearch.clear();
                quadTree.search(separationArea, positions, seperationSearch, settings.ZOMBIE_RADIUS);

                seperation = calcSeperation(i, seperationSearch, positions);
            }

            const float seperationForce = 5.f;
            
            seperation.x *= seperationForce;
            seperation.y *= seperationForce;

            Vector2 cohesion = {0.f, 0.f};
            if (cohesionEnabled) {
                cohesionArea.x = positions.xPos[i] - ((cohesionArea.width / 2.f) - settings.ZOMBIE_RADIUS);
                cohesionArea.y = positions.yPos[i] - ((cohesionArea.height / 2.f) - settings.ZOMBIE_RADIUS);
                cohesionSearch.clear();
                quadTree.search(cohesionArea, positions, cohesionSearch, settings.ZOMBIE_RADIUS);
                cohesion = calcCohesion(i, cohesionSearch, positions);
            }

            const float cohesionForce = 0.0015f;

            cohesion.x *= cohesionForce;
            cohesion.y *= cohesionForce;

            // set force
            Vector2 acceleration = {
                alignment.x + seperation.x + cohesion.x,
                alignment.y + seperation.y + cohesion.y
            };

            float maxAcc = 5.f;
            acceleration.x = MathHack::clamp(acceleration.x, -maxAcc, maxAcc);
            acceleration.y = MathHack::clamp(acceleration.y, -maxAcc, maxAcc);
            
            if((acceleration.x > 0.f || acceleration.y < 0.f) && (acceleration.y > 0.f || acceleration.y < 0.f)){
                float length = sqrtf(acceleration.x * acceleration.x) + (acceleration.y * acceleration.y);
                acceleration.x /= length;
                acceleration.y /= length;
            }

            speeds.accX[i] = acceleration.x;
            speeds.accY[i] = acceleration.y;
        }
    }

  private:
};

#endif
