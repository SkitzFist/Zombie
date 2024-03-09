#ifndef ZOMBIE_QUAD_TREE_H_
#define ZOMBIE_QUAD_TREE_H_

#include <memory>
#include <list>
#include <array>

#include "raylib.h"
#include "Entity.h"
#include "CollisionDetection.h"

inline constexpr const int MAX_LEVEL = 3;

/*
    Childs go in order north-west, north-east, south-west and south-east
*/

struct QuadTree{
    int level;
    Rectangle rect;
    
    std::array<std::unique_ptr<QuadTree>, 4> children;
    Rectangle childRects[4];

    std::list<Entity> entities; // maybe switch to another datastructure, needs to keep memory adress intact on insert.

    QuadTree(int level, const Rectangle& rect): 
        level(level), rect(rect){
        
        const float halfWidth = rect.width / 2;
        const float halfHeight = rect.height / 2;
        childRects[0] = {
            rect.x, rect.y,
            halfWidth, halfHeight
        };

        childRects[1] = {
            (rect.x + halfWidth), rect.y,
            halfWidth, halfHeight
        };

        childRects[2] = {
            rect.x, (rect.y + halfHeight),
            halfWidth, halfHeight
        };

        childRects[3] = {
            (rect.x + halfWidth), (rect.y + halfHeight),
            halfWidth, halfHeight
        };
    }

    void add(const Entity& entity, const Rectangle& entityRect){
        for(size_t i = 0; i < children.size(); ++i){
            //if child exists and fits entirely, add it to child
            if(children[i] && rectangleContains(children[i]->rect, entityRect)){
                children[i]->add(entity, entityRect);
                return;
            } else if(rectangleContains(childRects[i], entityRect)){
                // if entity fits inside child but it doesn ot exist, create child and add entity
                children[i] = std::make_unique<QuadTree>(level+1, childRects[i]);
                children[i]->add(entity, entityRect);
                return;
            }
        }

        //if no child can contain entity, add to this node
        entities.push_back(entity);
    }

    void clear(){
        entities.clear();

        for(auto& child : children){
            if(child){
                child->clear();
            }
        }
    }

    std::list<Entity> search(const Rectangle& area)const{
        std::list<Entity> items;
        search(area, items);
        return items;
    }

    void search(const Rectangle& area, std::list<Entity>& items)const{
    
        for(const auto& entity : entities){
            //todo should check if overlaps
            items.push_back(entity);
        }

        for(size_t i = 0; i < children.size(); ++i){
            
            if(children[i]){
                if(rectangleContains(area, children[i]->rect)){
                    //if search area contains an entire child, retrieve all items from that child
                    children[i]->items(items);

                }else if(rectangleOverlaps(children[i]->rect, area)){
                    children[i]->search(area, items);
                }
            }
        }
    }

    void items(std::list<Entity>& items)const{
        for(const auto& entity : entities){
            items.push_back(entity);
        }

        for(size_t i = 0; i < children.size(); ++i){

            if(children[i]){

                children[i]->items(items);
            }
        }
    }

    ~QuadTree(){
        //destroy all childs
    }

};

#endif
