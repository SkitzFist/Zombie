#ifndef _Game
#define _Game

#include <vector>

#include "raylib.h"
#include "entt/entt.hpp"
#include "CameraInput.hpp"

class Game{
public:
    Game(int screenWidth, int screenHeight, bool isFullscreen);
    ~Game();

    void init();

    void run();
    void webRun();

private:
    Camera2D m_camera;
    CameraInput m_cameraInput;
    entt::registry m_registry;

    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems();

    void initiateEntities();
    
    void drawZombies();
    void drawGrid() const; //debug, should be its own system
    void drawUi() const;
};

#endif