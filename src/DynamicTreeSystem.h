#ifndef ZOMBIE_DYNAMIC_TREE_SYSTEM_H_
#define ZOMBIE_DYNAMIC_TREE_SYSTEM_H_

#include <memory>
#include <vector>

#include "CollisionDetection.h"
#include "PositionComponent.h"
#include "QuadTree.h"
#include "Settings.h"
#include "ThreadPool.h"

// debug
#include "Log.hpp"

struct DynamicTreeSystem {
    bool isEnabled = true;

    std::vector<QuadTree *> entityToTreeLookup;
    std::vector<int> entitiesToReinsert;
    std::vector<int> entityToTreeIndexlookup;

    int currentIndex;
    int batchSize;

    DynamicTreeSystem(Settings &settings) : entityToTreeLookup(settings.MAX_ENTITIES),
                                            entitiesToReinsert(settings.MAX_ENTITIES),
                                            currentIndex(0),
                                            batchSize(settings.MAX_ENTITIES / 4) {
        entityToTreeIndexlookup.resize(settings.MAX_ENTITIES);
    }

    void update(Settings &settings, PositionComponent &positions, QuadTree &quadTree, std::vector<int> &entitiesOutOfBounds) {
        if (!isEnabled) {
            return;
        }

        entitiesToReinsert.clear();

        if (currentIndex == settings.MAX_ENTITIES) {
            currentIndex = 0;
        }

        int batch = currentIndex + batchSize;

        if (batch >= settings.MAX_ENTITIES) {
            batch = settings.MAX_ENTITIES;
        }

        Rectangle rect;
        rect.width = settings.ZOMBIE_RADIUS * 2.f;
        rect.height = settings.ZOMBIE_RADIUS * 2.f;
        for (; currentIndex < batch; ++currentIndex) {
            if (entityToTreeLookup[currentIndex] == nullptr) {
                continue;
            }

            rect.x = positions.xPos[currentIndex];
            rect.y = positions.yPos[currentIndex];
            if (!rectangleOverlaps(entityToTreeLookup[currentIndex]->rect, rect)) {
                entitiesToReinsert.push_back(currentIndex);
            }
        }

        // remove entities
        for (std::size_t i = 0; i < entitiesToReinsert.size(); ++i) {
            int entityID = entitiesToReinsert[i];
            QuadTree *tree = entityToTreeLookup[entityID];
            std::swap(tree->entities[entityToTreeIndexlookup[entityID]], tree->entities.back());
            tree->entities.pop_back();
        }

        for (std::size_t i = 0; i < entitiesToReinsert.size(); ++i) {
            int entityID = entitiesToReinsert[i];
            rect.x = positions.xPos[entityID];
            rect.y = positions.yPos[entityID];
            quadTree.add(entityID, rect, entityToTreeLookup, entityToTreeIndexlookup, entitiesOutOfBounds);
        }
    }
};

#endif
