#ifndef ZOMBIE_GRID_H_
#define ZOMBIE_GRID_H_

#include <vector>

#include "raylib.h"
#include "Entity.h"

//debug
#include "Log.hpp"

struct Node{
    std::vector<Entity> entities;
};

struct Grid{
    std::vector<Node> grid;

    Grid(int width, int height, int tileSize){
        int nodesX = width / tileSize;
        int nodesY = height / tileSize;
        int totalSize = nodesX * nodesY;
        grid.reserve(totalSize);
        
        Log::info(("Total number of cells: " + std::to_string(totalSize)).c_str());

    }

    void insert(Entity& entity, float x, float y){
    }
};

#endif