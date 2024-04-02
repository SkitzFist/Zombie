#ifndef ZOMBIE_SIMPLE_OUT_OF_BOUNDS_SYSTEM_H
#define ZOMBIE_SIMPLE_OUT_OF_BOUNDS_SYSTEM_H

#include <vector>

#include "DynamicTreeSystem.h"
#include "PositionComponent.h"
#include "QuadTree.h"
#include "Settings.h"
#include "World.hpp"
#include "raylib.h"

struct SimpleOutOfBoundSystem {
    std::vector<int> entitiesOutOfBounds;
    std::vector<int> reinsert;
    bool isEnabled = true;

    SimpleOutOfBoundSystem(Settings &settings){
      entitiesOutOfBounds.reserve(settings.MAX_ENTITIES / 2);
      reinsert.reserve(settings.MAX_ENTITIES / 2);
    }

    void update(Settings &settings, QuadTree &tree, DynamicTreeSystem &dynamicTreeSystem, PositionComponent &positions, WorldBounds &world) {

        if (!isEnabled || entitiesOutOfBounds.empty()) {
            return;
        }
        
        for (std::size_t i = 0; i < entitiesOutOfBounds.size(); ++i) {
            int id = entitiesOutOfBounds[i];
            float xPos = positions.xPos[id];
            float yPos = positions.yPos[id];

            if (xPos < 0.f) {
               positions.xPos[id] = world.width - (settings.ZOMBIE_RADIUS * 2.f);
            } else if (xPos > world.width) {
               positions.xPos[id] = 0.f;
            }

            if (yPos < 0.f) {
               positions.yPos[id] = world.height - (settings.ZOMBIE_RADIUS * 2.f);
            } else if (yPos > world.height) {
               positions.yPos[id] = 0.f;
            }

            reinsert.push_back(id);
        }
        entitiesOutOfBounds.clear();

        // reinsert

        Rectangle rect;
        rect.width = settings.ZOMBIE_RADIUS * 2.f;
        rect.height = settings.ZOMBIE_RADIUS * 2.f;
        for (const int &id : reinsert) {
            rect.x = positions.xPos[id];
            rect.y = positions.yPos[id];

            tree.add(id, rect, dynamicTreeSystem.entityToTreeLookup, dynamicTreeSystem.entityToTreeIndexlookup, entitiesOutOfBounds);
        }
        reinsert.clear();
    }
};

#endif
