#ifndef ZOMBIE_OUT_OF_BOUNDS_SYSTEM_H_
#define ZOMBIE_OUT_OF_BOUNDS_SYSTEM_H_

#include "PositionComponent.h"
#include "QuadTree.h"
#include "SearchResult.h"
#include "Settings.h"
#include "ThreadPool.h"
#include "World.hpp"

inline void searchTree(QuadTree &tree, SearchResult &search, Rectangle &searchArea, PositionComponent &positions) {
    tree.search(searchArea, positions, search, 32.f);
}

inline void correctOutOfBoundsX(SearchResult &search, WorldBounds &world, PositionComponent &positions) {
    for(int i = 0; i < search.size; ++i){
      float x = positions.xPos[search.arr[i]];
      if(x < 0.f){
        x = world.width;
        positions.xPos[search.arr[i]] = x;
      }else if(x > world.width){
        x = 0.f;
        positions.xPos[search.arr[i]] = x;
      }
    }
}

inline void correctOutOfBoundsY(SearchResult &search, WorldBounds &world, PositionComponent &positions) {
    for (int i = 0; i < search.size; ++i) {
      float y = positions.yPos[search.arr[i]];
      if (y < 0.f) {
        y = world.height;
        positions.yPos[search.arr[i]] = y;
      } else if (y > world.height) {
        y = 0.f;
        positions.yPos[search.arr[i]] = y;
      }
    }
}

struct OutOfBoundsSystem {
    bool isEnabled = false;
    SearchResult leftWall;
    SearchResult rightWall;
    SearchResult topWall;
    SearchResult botWall;

    Rectangle left;
    Rectangle right;
    Rectangle top;
    Rectangle bot;

    OutOfBoundsSystem(const Settings &settings,
                      const WorldBounds &world) : leftWall(100000),
                                                  rightWall(100000),
                                                  topWall(100000),
                                                  botWall(100000) {

        float wallThickness = settings.ZOMBIE_RADIUS * 2.f;

        left = {
            0.f,
            0.f,
            wallThickness,
            (float)world.height};

        right = {
            world.width - wallThickness,
            0.f,
            wallThickness,
            (float)world.height};

        top = {
            wallThickness,
            0.f,
            (float)world.width - (wallThickness * 2.f),
            wallThickness};

        bot = {
            wallThickness,
            (float)world.height - wallThickness,
            (float)world.width - (wallThickness * 2.f),
            wallThickness};
    }

    void search(QuadTree &tree, ThreadPool &threadPool, PositionComponent &positions) {
        if(!this->isEnabled){
          return;
        }
        threadPool.enqueue(searchTree, std::ref(tree), std::ref(leftWall), std::ref(left), std::ref(positions));
        threadPool.enqueue(searchTree, std::ref(tree), std::ref(rightWall), std::ref(right), std::ref(positions));
        threadPool.enqueue(searchTree, std::ref(tree), std::ref(topWall), std::ref(top), std::ref(positions));
        threadPool.enqueue(searchTree, std::ref(tree), std::ref(botWall), std::ref(bot), std::ref(positions));
    }

    void update(ThreadPool &threadPool, PositionComponent &positions, WorldBounds& world) {

      if(!this->isEnabled){
        return;
      }
      
      if(leftWall.size > 0){
        threadPool.enqueue(correctOutOfBoundsX, std::ref(leftWall), std::ref(world), std::ref(positions));
        threadPool.enqueue(correctOutOfBoundsY, std::ref(leftWall), std::ref(world), std::ref(positions));
      }

      if(rightWall.size > 0){
        threadPool.enqueue(correctOutOfBoundsX, std::ref(rightWall), std::ref(world), std::ref(positions));
        threadPool.enqueue(correctOutOfBoundsY, std::ref(rightWall), std::ref(world), std::ref(positions));
      }

      if(topWall.size > 0){
        threadPool.enqueue(correctOutOfBoundsX, std::ref(topWall), std::ref(world), std::ref(positions));
        threadPool.enqueue(correctOutOfBoundsY, std::ref(topWall), std::ref(world), std::ref(positions));
      }

      if(botWall.size > 0){
        threadPool.enqueue(correctOutOfBoundsX, std::ref(botWall), std::ref(world), std::ref(positions));
        threadPool.enqueue(correctOutOfBoundsY, std::ref(botWall), std::ref(world), std::ref(positions));
      }

      leftWall.clear();
      rightWall.clear();
      topWall.clear();
      botWall.clear();

    }
};

#endif
