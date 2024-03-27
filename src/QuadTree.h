#ifndef ZOMBIE_QUAD_TREE_H_
#define ZOMBIE_QUAD_TREE_H_

#include <array>
#include <memory>
#include <vector>

#include "CollisionDetection.h"
#include "PositionComponent.h"
#include "SearchResult.h"
#include "raylib.h"

inline constexpr const int MAX_LEVEL = 8;

/*
    Childs go in order north-west, north-east, south-west and south-east
*/

struct QuadTree {
    int level;
    Rectangle rect;

    std::array<std::shared_ptr<QuadTree>, 4> children;
    Rectangle childRects[4];

    std::vector<int> entities;
    Rectangle zombieRect;

    QuadTree(int level, const Rectangle &rect) : level(level), rect(rect) {

        const float halfWidth = rect.width / 2;
        const float halfHeight = rect.height / 2;
        childRects[0] = {
            rect.x, rect.y,
            halfWidth, halfHeight};

        childRects[1] = {
            (rect.x + halfWidth), rect.y,
            halfWidth, halfHeight};

        childRects[2] = {
            rect.x, (rect.y + halfHeight),
            halfWidth, halfHeight};

        childRects[3] = {
            (rect.x + halfWidth), (rect.y + halfHeight),
            halfWidth, halfHeight};
    }

    void add(const int &entityId, const Rectangle &entityRect, std::vector<QuadTree *> &entityToTreeLookup, std::vector<int> &entityToTreeIndexLookup, std::vector<int>& entitiesOutOfBounds) {

        if (level == 0) {
            if (!rectangleContains(rect, entityRect)) {
                entitiesOutOfBounds.push_back(entityId);
                entityToTreeLookup[entityId] = nullptr;
                entityToTreeIndexLookup[entityId] = -1;
                return;
            }
        }

        if (level == MAX_LEVEL) {
            entities.push_back(entityId);
            entityToTreeLookup[entityId] = this;
            entityToTreeIndexLookup[entityId] = entities.size() - 1;
            return;
        }

        for (size_t i = 0; i < children.size(); ++i) {
            // if child exists and fits entirely, add it to child
            if (children[i] && rectangleOverlaps(children[i]->rect, entityRect)) {
                children[i]->add(entityId, entityRect, entityToTreeLookup, entityToTreeIndexLookup, entitiesOutOfBounds);
                return;
            } else if (rectangleOverlaps(childRects[i], entityRect)) {
                // if entity fits inside child but child it does not exist, create child and add entity
                children[i] = std::make_shared<QuadTree>(level + 1, childRects[i]);
                children[i]->add(entityId, entityRect, entityToTreeLookup, entityToTreeIndexLookup, entitiesOutOfBounds);
                return;
            }
        }

        /*
        // if no child can contain entity, add to this node
        entities.push_back(entityId);
        entityToTreeLookup[entityId] = this;
        entityToTreeIndexLookup[entityId] = entities.size() - 1;
        */
    }

    void clear() {
        entities.clear();

        for (auto &child : children) {
            if (child) {
                child->clear();
            }
        }
    }

    void search(const Rectangle &area, const PositionComponent &position, SearchResult &searchResult, const float radius) {

        for (const auto &entityID : entities) {
            Vector2 pos = {position.xPos[entityID], position.yPos[entityID]};
            zombieRect = {pos.x, pos.y, radius * 2, radius * 2};
            if (rectangleOverlaps(area, zombieRect)) {
                searchResult.add(entityID);
            }
        }

        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]) {
                if (rectangleContains(area, children[i]->rect)) {
                    children[i]->getAll(searchResult);
                } else if (rectangleOverlaps(area, children[i]->rect)) {
                    children[i]->search(area, position, searchResult, radius);
                }
            }
        }
    }

    void getAll(SearchResult &SearchResult) {
        for (const auto &entityID : entities) {
            SearchResult.add(entityID);
        }

        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]) {
                children[i]->getAll(SearchResult);
            }
        }
    }

    void draw(Rectangle& cameraRect) const {
        
        if((!entities.empty() && rectangleOverlaps(cameraRect, rect)) || level == 0){
            Color c = (level % 2) == 0 ? RAYWHITE : YELLOW;
            DrawRectangleLinesEx(rect, 10.f, c);
        }

        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]) {
                children[i]->draw(cameraRect);
            }
        }
    }

    ~QuadTree() {
        // todo proper cleanup
    }
};

#endif
