#ifndef _VelocityComponent_h_
#define _VelocityComponent_h_

#include "raylib.h"

struct VelocityComponent{
    Vector2 velocity;
    Vector2 acceleration;

    VelocityComponent(const Vector2& velocity, const Vector2& acceleration){
        this->velocity = velocity;
        this->acceleration = acceleration;
    }
};

#endif