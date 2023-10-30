#ifndef _Game
#define _Game

#include <vector>

#include "raylib.h"
#include "ECS/Component/CircleShapeComponent.hpp"
#include "CameraInput.hpp"

class Game{
public:
    Game(int screenWidth, int screenHeight, bool isFullscreen);
    ~Game();

    void init();

    void run();
    void webRun();

private:
    unsigned int m_nrOfEntities;
    std::vector<Entity> m_entities;
    CircleShapeComponents circleShapeComponents;
    unsigned int m_indexUpdate;
    Camera2D m_camera;
    CameraInput m_cameraInput;

    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems();

    void initiateEntities();
    
    void drawUi() const;
    void drawGrid() const; //debug, should be its own system
};

#endif