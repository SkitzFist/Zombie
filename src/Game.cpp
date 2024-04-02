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
                                                                   m_world({screenWidth * m_settings.WORLD_SCALE, screenHeight * m_settings.WORLD_SCALE, 32}),
                                                                   m_camera(),
                                                                   m_tree(0, {0, 0, (float)screenWidth * m_settings.WORLD_SCALE, (float)screenHeight * m_settings.WORLD_SCALE}),
                                                                   m_searchResult(m_settings.MAX_ENTITIES),
                                                                   m_threadPool(16),
                                                                   m_positions(m_settings),
                                                                   m_speeds(m_settings),
                                                                   m_boids(m_settings),
                                                                   m_zombieFactory(m_tree, m_positions, m_speeds, m_settings),
                                                                   m_dynamicTreeSystem(m_settings),
                                                                   m_simpleOutOfBoundsSystem(m_settings),
                                                                   m_boidSystem() {

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
        m_threadPool.awaitCompletion();
        float dt = GetFrameTime();
        handleInputSystems();
        handleUpdateSystems(dt);
        handleRenderSystems();
    }
}

void Game::webRun() {
    float dt = GetFrameTime();
    m_threadPool.awaitCompletion();
    handleInputSystems();
    handleUpdateSystems(dt);
    handleRenderSystems();
}

void Game::handleInputSystems() {
    m_cameraInput.handleInput(m_camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, m_camera);
        m_zombieFactory.createZombie(worldPos.x, worldPos.y, 0.005f, 0.f, m_dynamicTreeSystem.entityToTreeLookup, m_dynamicTreeSystem.entityToTreeIndexlookup, m_simpleOutOfBoundsSystem.entitiesOutOfBounds);
    }

    if (IsKeyPressed(KEY_M)) {
        m_moveSystem.isEnabled = !m_moveSystem.isEnabled;
    } else if (IsKeyPressed(KEY_K)) {
        m_dynamicTreeSystem.isEnabled = !m_dynamicTreeSystem.isEnabled;
    } else if (IsKeyPressed(KEY_O)) {
        m_simpleOutOfBoundsSystem.isEnabled = !m_simpleOutOfBoundsSystem.isEnabled;
    } else if (IsKeyPressed(KEY_B)) {
        m_boidSystem.isEnabled = !m_boidSystem.isEnabled;
    } else if (IsKeyPressed(KEY_ONE)) {
        m_boidSystem.alignmentEnabled = !m_boidSystem.alignmentEnabled;
    } else if (IsKeyPressed(KEY_TWO)) {
        m_boidSystem.separationEnabled = !m_boidSystem.separationEnabled;
    } else if (IsKeyPressed(KEY_THREE)) {
        m_boidSystem.cohesionEnabled = !m_boidSystem.cohesionEnabled;
    }
}

void Game::handleUpdateSystems(float dt) {
    (void)dt;

    if (currentIndex >= (m_settings.MAX_ENTITIES - 1)) {

        m_boidSystem.update(m_settings, m_threadPool, m_tree, m_positions, m_speeds, m_boids);
        m_moveSystem.update(m_positions, m_speeds, m_threadPool);
        m_threadPool.awaitCompletion();

        m_dynamicTreeSystem.update(m_settings, m_positions, m_tree, m_simpleOutOfBoundsSystem.entitiesOutOfBounds);
        m_simpleOutOfBoundsSystem.update(m_settings, m_tree, m_dynamicTreeSystem, m_positions, m_world);
        return;
    }

    int maxBatch = currentIndex + maxBatchSize;
    if (maxBatch > m_settings.MAX_ENTITIES) {
        maxBatch = m_settings.MAX_ENTITIES;
    }
    for (; currentIndex < maxBatch; ++currentIndex) {
        float randX = GetRandomValue(0, m_world.width);
        float randY = GetRandomValue(0, m_world.height);

        float randomAccX = GetRandomValue(-5, 5);
        float randomAccY = GetRandomValue(-5, 5);
        m_zombieFactory.createZombie(randX,
                                     randY,
                                     randomAccX,
                                     randomAccY,
                                     m_dynamicTreeSystem.entityToTreeLookup,
                                     m_dynamicTreeSystem.entityToTreeIndexlookup,
                                     m_simpleOutOfBoundsSystem.entitiesOutOfBounds);
    }
}

Rectangle Game::getCameraRect() const {
    Vector2 cameraPos = m_camera.target;
    Vector2 size = {GetScreenWidth() / m_camera.zoom, GetScreenHeight() / m_camera.zoom};
    return {cameraPos.x, cameraPos.y, size.x, size.y};
}

void Game::handleRenderSystems() {
    ClearBackground(BLACK);

    BeginDrawing();
    BeginMode2D(m_camera);

    Rectangle cameraRect = getCameraRect();

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        m_tree.draw(cameraRect);
    }

    // Draw zombies
    m_searchResult.clear();
    m_tree.search(cameraRect, m_positions, m_searchResult, m_settings.ZOMBIE_RADIUS);
    drawZombies(m_searchResult, m_positions, m_renderTexture.texture);

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
    int fontSize = 27;
    int spacing = 30;
    int cSpacing = spacing / 3;
    int xPos = 10;

    std::string fpsStr = "FPS: " + std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string countStr = "Entities drawn: " + std::to_string(m_searchResult.size) + " / " + std::to_string(currentIndex);
    DrawText(countStr.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string dynamicTree = "Dynamic Tree: " + std::to_string(m_dynamicTreeSystem.isEnabled);
    DrawText(dynamicTree.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string outOfBounds = "OutOfBounds: " + std::to_string(m_simpleOutOfBoundsSystem.isEnabled);
    DrawText(outOfBounds.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string moveSystem = "MoveSystem: " + std::to_string(m_moveSystem.isEnabled);
    DrawText(moveSystem.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string boidsSystem = "BoidSystem: " + std::to_string(m_boidSystem.isEnabled);
    DrawText(boidsSystem.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string alignment = "alignment: " + std::to_string(m_boidSystem.alignmentEnabled);
    DrawText(alignment.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string seperation = "seperation: " + std::to_string(m_boidSystem.separationEnabled);
    DrawText(seperation.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;

    std::string cohesion = "cohesion: " + std::to_string(m_boidSystem.cohesionEnabled);
    DrawText(cohesion.c_str(), xPos, cSpacing, fontSize, WHITE);
    cSpacing += spacing;
}
