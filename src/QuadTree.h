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

    std::array<std::unique_ptr<QuadTree>, 4> children;
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

    void add(const int &entityId, const Rectangle &entityRect) {
        if (level == MAX_LEVEL) {
            entities.push_back(entityId);
            return;
        }

        for (size_t i = 0; i < children.size(); ++i) {
            // if child exists and fits entirely, add it to child
            if (children[i] && rectangleContains(children[i]->rect, entityRect)) {
                children[i]->add(entityId, entityRect);
                return;
            } else if (rectangleContains(childRects[i], entityRect)) {
                // if entity fits inside child but child it does not exist, create child and add entity
                children[i] = std::make_unique<QuadTree>(level + 1, childRects[i]);
                children[i]->add(entityId, entityRect);
                return;
            }
        }

        // if no child can contain entity, add to this node
        entities.push_back(entityId);
    }

    void clear() {
        entities.clear();

        for (auto &child : children) {
            if (child) {
                child->clear();
            }
        }
    }

    
    void search(const Rectangle &area, const PositionComponent &position, SearchResult &searchResult, const float radius){

        for (const auto &entityID : entities) {
            Vector2 pos = {position.xPos[entityID], position.yPos[entityID]};
            zombieRect = {pos.x - radius, pos.y - radius, radius*2, radius*2};
            if(rectangleOverlaps(area, zombieRect)){
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

    void draw() const {

        Color c = (level % 2) == 0 ? RAYWHITE : YELLOW;
        DrawRectangleLinesEx(rect, 10.f, c);

        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]) {
                children[i]->draw();
            }
        }
    }

    ~QuadTree() {
        // todo proper cleanup
    }
};

#endif
