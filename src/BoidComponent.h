#ifndef ZOMBIE_BOID_COMPONENT_H_
#define ZOMBIE_BOID_COMPONENT_H_

#include <vector>

#include "Settings.h"
#include "raylib.h"

struct BoidComponent {
    std::vector<Vector2> alignments;
    std::vector<Vector2> separations;
    std::vector<Vector2> cohesions;

    BoidComponent(Settings &settings) {
        alignments.reserve(settings.MAX_ENTITIES);
        separations.reserve(settings.MAX_ENTITIES);
        cohesions.reserve(settings.MAX_ENTITIES);
    }

    void add() {
        alignments.push_back({0.f, 0.f});
        separations.push_back({0.f, 0.f});
        cohesions.push_back({0.f, 0.f});
    }
};

#endif