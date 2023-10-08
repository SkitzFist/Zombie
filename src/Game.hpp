#ifndef _Game
#define _Game

#include <vector>

#include "../include/raylib.h"
#include "ECS/Component/CircleShapeComponent.hpp"

class Game{
public:
    Game(int screenWidth, int screenHeight, bool isFullscreen);
    ~Game();

    void init();

    void run();
    void webRun();
private:
    unsigned int nrOfEntities;
    std::vector<Entity> entities;
    CircleShapeComponents circleShapeComponents;
    unsigned int indexUpdate;

    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems();

    void initiateEntities();
    
    void drawUi();
};

#endif