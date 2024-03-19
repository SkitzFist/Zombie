#ifndef _Game
#define _Game

#include "raylib.h"
#include "ThreadPool.h"
#include "World.hpp"
#include "CameraInput.hpp"

#include "PositionComponent.h"
#include "SpeedComponent.h"

#include "QuadTree.h"
#include "SearchResult.h"

#include "ZombieFactory.h"

#include "MoveSystem.h"
#include "OutOfBoundsSystem.h"

class Game{
public:
    Game(int screenWidth, int screenHeight, bool isFullscreen);
    ~Game();

    void init();

    void run();
    void webRun();

private:
    Settings m_settings;
    WorldBounds m_world;
    Camera2D m_camera;
    CameraInput m_cameraInput;
    
    QuadTree m_tree;
    SearchResult m_searchResult;
    ThreadPool m_threadPool;
    
    PositionComponent m_positions;
    SpeedComponent speedComponent;

    RenderTexture2D m_renderTexture;
    ZombieFactory m_zombieFactory;
    
    MoveSystem m_moveSystem;
    OutOfBoundsSystem m_outOfboundsSystem;

    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems();

    void drawGrid() const; //debug, should be its own system
    void drawUi() const;
    void drawZombie();

    //debug
    Rectangle getCameraRect() const;

    int currentIndex = 0;
    int maxBatchSize = 10000;
};

#endif