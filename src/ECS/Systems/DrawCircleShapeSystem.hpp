#ifndef _DrawCircleShapeSystem
#define _DrawCircleShapeSystem

#include "System.hpp"
#include "../Component/CircleShapeComponent.hpp"
#include "../Component/Transform.hpp"

//debug
#include "../../Log.hpp"

class DrawCircleShapeSystem{
public:
    DrawCircleShapeSystem(const CircleShapeComponents& circleShapeComponent, const TransformComponent& transformComponent):
        m_circleShapecomponent(circleShapeComponent),
        m_transform(transformComponent){
        
    }

    void renderAll(){
        
    }

private:
    SystemProperties m_sysProps;
    CircleShapeComponents m_circleShapecomponent;
    TransformComponent m_transform;
};

#endif