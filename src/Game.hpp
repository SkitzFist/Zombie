#ifndef _Game
#define _Game

#include "raylib.h"
#include "World.hpp"
#include "CameraInput.hpp"
#include "PositionComponent.h"
#include "QuadTree.h"

class Game{
public:
    Game(int screenWidth, int screenHeight, bool isFullscreen);
    ~Game();

    void init();

    void run();
    void webRun();

private:
    WorldBounds m_world;
    Camera2D m_camera;
    CameraInput m_cameraInput;
    QuadTree m_tree;
    //std::vector<Entity> m_entities;
    PositionComponent positionComponent;

    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems();
    
    void drawGrid() const; //debug, should be its own system
    void drawUi() const;
    void drawZombie() const;

    //debug
    Rectangle getCameraRect() const;

    int currentIndex = 0;
    int maxBatchSize = 10000;
};

#endif