#ifndef ZOMBIE_ZOMBIE_FACTORY_H_
#define ZOMBIE_ZOMBIE_FACTORY_H_

#include "PositionComponent.h"
#include "SpeedComponent.h"
#include "QuadTree.h"
#include "Settings.h"
#include "raylib.h"

struct ZombieFactory {
    QuadTree &tree;
    PositionComponent &positions;
    SpeedComponent &speeds;
    Settings &settings;
    int ID;
    ZombieFactory(QuadTree &tree, 
                  PositionComponent &positions,
                  SpeedComponent &speeds,
                  Settings &settings) : tree(tree), 
                                        positions(positions),
                                        speeds(speeds),
                                        settings(settings), 
                                        ID(0) {}


    void createZombie(const float x, const float y, const float accX, const float accY, std::vector<QuadTree*>& entityToTreeLookup, std::vector<int>& entityToTreeIndexlookup, std::vector<int>& entitiesOutOfBounds){
      if(ID >= settings.MAX_ENTITIES){
        return;
      }

      positions.add(x, y, ID);
      speeds.add(accX, accY, 0.f, 0.f);
      /*
      Rectangle zombieRect = {
        x - (settings.ZOMBIE_RADIUS / 2.f),
        y - (settings.ZOMBIE_RADIUS / 2.f),
        settings.ZOMBIE_RADIUS * 2.f,
        settings.ZOMBIE_RADIUS * 2.f
      };
      */

      Rectangle zombieRect = {
          x - settings.ZOMBIE_RADIUS,
          y - settings.ZOMBIE_RADIUS,
          settings.ZOMBIE_RADIUS * 2.f,
          settings.ZOMBIE_RADIUS * 2.f};

      tree.add(ID, zombieRect, entityToTreeLookup, entityToTreeIndexlookup, entitiesOutOfBounds);

      ++ID;
    }
};

#endif
