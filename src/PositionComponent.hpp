#ifndef _PositionComponent
#define _PositionComponent

#include "raylib.h"

struct PositionComponent
{
    Vector2 position;

    PositionComponent(const Vector2& position):
        position(position){}
};


#endif