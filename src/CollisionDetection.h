#ifndef ZOMBIE_COLLISION_DETECTION_H_
#define ZOMBIE_COLLISION_DETECTION_H_

#include "Log.hpp"
#include "cmath"
#include "raylib.h"

inline bool rectangleContains(const Rectangle &container, const Rectangle &containee) {
    return containee.x > container.x && (containee.x + containee.width) < (container.x + container.width) &&
           containee.y > container.y && (containee.y + containee.height) < (container.y + container.height);
}

inline bool rectangleOverlaps(const Rectangle &container, const Rectangle &colapsee) {
    return colapsee.x < (container.x + container.width) && (colapsee.x + colapsee.width) > container.x &&
           colapsee.y < (container.y + container.height) && (colapsee.y + colapsee.height) > container.y;
}

inline bool rectangleCircleContains(const Rectangle &container, const Vector2 &point) {
    return point.x > container.x && point.x < (container.x + container.width) &&
           point.y > container.y && point.y < (container.y + container.height);
}

inline bool rectangleCircleOverlaps(const Rectangle &container, const Vector2 &point, float radius) {
    Vector2 rectMid = {
        container.x + (container.width / 2),
        container.y + (container.height / 2)};

    Vector2 distV = {rectMid.x - point.x, rectMid.y - point.y};
    float dist = sqrtf((distV.x * distV.x) + (distV.y * distV.y)); // todo should use fast inverse sqr root

    return ((container.width / 2) + radius > dist) ||
           ((container.height / 2) + radius > dist);
}

#endif
