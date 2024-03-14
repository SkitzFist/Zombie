#ifndef ZOMBIE_ZOMBIE_FACTORY_H_
#define ZOMBIE_ZOMBIE_FACTORY_H_

#include "PositionComponent.h"
#include "QuadTree.h"
#include "Settings.h"
#include "raylib.h"

struct ZombieFactory {
    QuadTree &tree;
    PositionComponent &positions;
    Settings &settings;
    int ID;
    ZombieFactory(QuadTree &tree, 
                  PositionComponent &positions, 
                  Settings &settings) : tree(tree), 
                                        positions(positions), 
                                        settings(settings), 
                                        ID(0) {}


    void createZombie(const float x, const float y){
      if(ID >= settings.MAX_ENTITIES){
        return;
      }

      positions.add(x, y, ID);
      
      Rectangle zombieRect = {
        x - (settings.ZOMBIE_RADIUS / 2.f),
        y - (settings.ZOMBIE_RADIUS / 2.f),
        settings.ZOMBIE_RADIUS * 2.f,
        settings.ZOMBIE_RADIUS * 2.f
      };

      tree.add(ID, zombieRect);

      ++ID;
    }
};

#endif
