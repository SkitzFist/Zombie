#ifndef _Game
#define _Game

#include <vector>

#include "../include/raylib.h"
#include "ECS/Component/CircleShapeComponent.hpp"

class Game{
public:
    Game();
    ~Game();

    void init();

    void run();
    void webRun();
private:
    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems() const;
    unsigned int nrOfEntities;
    std::vector<Entity> entities;
    CircleShapeComponents circleShapeComponents;
    unsigned int index;
    unsigned int indexUpdate;
};

#endif