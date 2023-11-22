#include "Game.hpp"

#include "World.hpp"

//debug
#include "Log.hpp"
#include "Timer.hpp"
#include <string>
#include <cmath>

Game::Game(int screenWidth, int screenHeight, bool isFullscreen): 
    m_camera(){
    #if PLATFORM_WEB
        InitWindow(screenWidth, screenHeight, "Zombie");
    #else
        InitWindow(0,0, "Zombie");
        SetWindowSize(screenWidth, screenHeight);
        if(isFullscreen){
            ToggleFullscreen();
        }

        Vector2 windowPos = GetWindowPosition();
        windowPos.y += 30;
        SetWindowPosition(windowPos.x, windowPos.y);
    #endif

    SetTargetFPS(144);

    m_camera.offset = {0,0};
    m_camera.target = {0,0};
    m_camera.rotation = 0.f;
    m_camera.zoom = 1.f;

    SetExitKey(KEY_ESCAPE);
}   

Game::~Game(){

}

void Game::run(){
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        handleInputSystems();
        handleUpdateSystems(dt);
        handleRenderSystems();
    }    
}

void Game::webRun(){
        float dt = GetFrameTime();
        handleInputSystems();
        handleUpdateSystems(dt);
        handleRenderSystems();
}

void Game::handleInputSystems(){
    m_cameraInput.handleInput(m_camera);
}

void Game::handleUpdateSystems(float dt){

}

void Game::handleRenderSystems(){
    ClearBackground(BLACK);

    BeginDrawing();
        BeginMode2D(m_camera);
            drawGrid();
        EndMode2D();

        
        drawUi(); // last
    EndDrawing();
}

void Game::drawUi()const{
    std::string fpsStr = std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), 10, 10, 20, WHITE);
}

void Game::drawGrid()const{
    
    Color color = RAYWHITE;
    color.a = 50;

    for(int i = 0; i <= World::WIDTH; i += World::TILE_SIZE){
        DrawLine(i, 0, i, World::HEIGHT, color);
    }

    for(int i = 0; i <= World::HEIGHT; i+= World::TILE_SIZE){
        DrawLine(0, i, World::WIDTH, i, color);
    }
}