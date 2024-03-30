#ifndef ZOMBIE_BOID_COMPONENT_H_
#define ZOMBIE_BOID_COMPONENT_H_

#include <vector>

#include "Settings.h"
#include "raylib.h"

struct BoidComponent {
    std::vector<Vector2> alignemnts;
    std::vector<Vector2> seperations;
    std::vector<Vector2> cohesions;

    BoidComponent(Settings &settings) {
        alignemnts.reserve(settings.MAX_ENTITIES);
        seperations.reserve(settings.MAX_ENTITIES);
        cohesions.reserve(settings.MAX_ENTITIES);
    }

    void add() {
        alignemnts.push_back({0.f, 0.f});
        seperations.push_back({0.f, 0.f});
        cohesions.push_back({0.f, 0.f});
    }
};

#endif