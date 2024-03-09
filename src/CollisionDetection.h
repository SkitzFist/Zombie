#ifndef ZOMBIE_COLLISION_DETECTION_H_
#define ZOMBIE_COLLISION_DETECTION_H_

#include "raylib.h"

inline bool rectangleContains(const Rectangle& container, const Rectangle& containee){
    return containee.x > container.x && (containee.x + containee.width) < (container.x + container.width) &&
           containee.y > container.y && (containee.y + containee.height) < (container.y + container.height);
}

inline bool rectangleOverlaps(const Rectangle& container, const Rectangle& colapsee){
    return colapsee.x < (container.x + container.width) && (colapsee.x + colapsee.width) > container.x &&
           colapsee.y < (container.y + container.height) && (colapsee.y + colapsee.height) > container.y;
}

#endif
