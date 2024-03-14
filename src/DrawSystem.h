#ifndef ZOMBIE_DRAW_SYSTEM_H_
#define ZOMBIE_DRAW_SYSTEM_H_

#include "PositionComponent.h"
#include "SearchResult.h"
#include "raylib.h"
#include "Log.hpp"

inline void drawZombies(const SearchResult& searchResult, const PositionComponent& positions, const Texture2D& texture){
  
  for(int i = 0; i < searchResult.size; ++i){
    Vector2 pos = positions.getPositionByIndex(searchResult.arr[i]);
    DrawTexture(texture, pos.x, pos.y, WHITE);
  }
}

#endif
