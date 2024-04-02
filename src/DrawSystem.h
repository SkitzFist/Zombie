#ifndef ZOMBIE_DRAW_SYSTEM_H_
#define ZOMBIE_DRAW_SYSTEM_H_

#include "PositionComponent.h"
#include "SearchResult.h"
#include "raylib.h"

inline void drawZombies(const SearchResult &searchResult,
                        const PositionComponent &positions,
                        const Texture2D &texture) {

    for (int i = 0; i < searchResult.size; ++i) {
        int entityID = searchResult.arr[i];

        DrawTexture(texture, positions.xPos[entityID], positions.yPos[entityID], WHITE);
    }
}

#endif
