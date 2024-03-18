#include "Game.hpp"

#include "CollisionDetection.h"
#include "ComponentTypes.h"
#include "DrawSystem.h"
#include "Entity.h"
#include "World.hpp"

// debug
#include "Log.hpp"
#include "MathHack.hpp"
#include "Timer.hpp"
#include <cmath>
#include <string>
Rectangle a1{0, 0, 30, 30};
Rectangle a2{200, 200, 100, 100};
// debug

Game::Game(int screenWidth, int screenHeight, bool isFullscreen) : m_settings(),
                                                                   m_camera(),
                                                                   m_tree(0, {0, 0, screenWidth * m_settings.WORLD_SCALE, screenHeight * m_settings.WORLD_SCALE}),
                                                                   positionComponent(m_settings),
                                                                   m_searchResult(m_settings.MAX_ENTITIES),
                                                                   m_zombieFactory(m_tree, positionComponent, m_settings),
                                                                   m_threadPool(8) {

#if PLATFORM_WEB
    InitWindow(screenWidth, screenHeight, "Zombie");
#else
    InitWindow(0, 0, "Zombie");
    SetWindowSize(screenWidth, screenHeight);
    if (isFullscreen) {
        ToggleFullscreen();
    }

    Vector2 windowPos = GetWindowPosition();
    windowPos.y += 50;
    windowPos.x += 50;
    SetWindowPosition(windowPos.x, windowPos.y);
#endif

    m_world.width = screenWidth * m_settings.WORLD_SCALE;
    m_world.height = screenHeight * m_settings.WORLD_SCALE;
    m_world.tileSize = 32;

    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    m_camera.offset = {0, 0};
    m_camera.target = {0, 0};
    m_camera.rotation = 0.f;
    m_camera.zoom = 1.f;

    m_renderTexture = LoadRenderTexture(m_settings.ZOMBIE_RADIUS * 2, m_settings.ZOMBIE_RADIUS * 2);
    BeginTextureMode(m_renderTexture);
        DrawCircle(m_renderTexture.texture.width / 2, m_renderTexture.texture.height / 2, m_settings.ZOMBIE_RADIUS, GREEN);
    EndTextureMode();
}

Game::~Game() {
}

void Game::run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInputSystems();
        handleUpdateSystems(dt);
        handleRenderSystems();
    }
}

void Game::webRun() {
    float dt = GetFrameTime();
    handleInputSystems();
    handleUpdateSystems(dt);
    handleRenderSystems();
}

void Game::handleInputSystems() {
    m_cameraInput.handleInput(m_camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, m_camera);
        m_zombieFactory.createZombie(worldPos.x, worldPos.y);      
    }

    if(IsKeyPressed(KEY_M)){
        m_moveSystem.isActive = !m_moveSystem.isActive;
    }
}

void Game::handleUpdateSystems(float dt) {
    (void)dt;

    if (currentIndex >= (m_settings.MAX_ENTITIES - 1)) {
        m_moveSystem.update(positionComponent, m_threadPool);
        return;
    }

    int maxBatch = currentIndex + maxBatchSize;
    if (maxBatch > m_settings.MAX_ENTITIES) {
        maxBatch = m_settings.MAX_ENTITIES - 1;
    }
    for (; currentIndex < maxBatch; ++currentIndex) {
        float randX = GetRandomValue(0, m_world.width);
        float randY = GetRandomValue(0, m_world.height);

        m_zombieFactory.createZombie(randX, randY);        
    }
}

Rectangle Game::getCameraRect() const {
    Vector2 cameraPos = m_camera.target; // GetScreenToWorld2D(m_camera.target, m_camera);
    Vector2 size = {GetScreenWidth() / m_camera.zoom, GetScreenHeight() / m_camera.zoom};
    return {cameraPos.x, cameraPos.y, size.x, size.y};
}

void Game::handleRenderSystems() {
    ClearBackground(BLACK);

    BeginDrawing();
    BeginMode2D(m_camera);

    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        m_tree.draw();
    }   

    //draw zombies
    Rectangle cameraRect = getCameraRect();
    m_searchResult.clear();
    m_tree.search(cameraRect, positionComponent, m_searchResult, m_settings.ZOMBIE_RADIUS);
    drawZombies(m_searchResult, positionComponent, m_renderTexture.texture);

    EndMode2D();

    drawUi(); // Allways draw last and outside of camera

    EndDrawing();
}

void Game::drawGrid() const {

    Color color = RAYWHITE;
    color.a = 50;

    for (int i = 0; i <= m_world.width; i += m_world.tileSize) {
        DrawLine(i, 0, i, m_world.height, color);
    }

    for (int i = 0; i <= m_world.height; i += m_world.tileSize) {
        DrawLine(0, i, m_world.width, i, color);
    }
}

void Game::drawUi() const {
    std::string fpsStr = std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), 10, 40, 40, WHITE);

    std::string countStr = "Entities drawn: " + std::to_string(m_searchResult.size);
    DrawText(countStr.c_str(), 10, 80, 40, WHITE);

    std::string maxEnt = "Total entities: " + std::to_string(currentIndex);
    DrawText(maxEnt.c_str(), 10, 120, 40, WHITE);
}
