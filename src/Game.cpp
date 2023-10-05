#include "Game.hpp"

#include "World.hpp"

//debug
#include "Log.hpp"

/*
    Notes: use sparse set for entity component mapping, use SOA for components.
*/

Game::Game(){
    InitWindow(World::WIDTH, World::HEIGHT, "Zombie");
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

}

void Game::handleUpdateSystems(float dt){

}

void Game::handleRenderSystems()const{
    ClearBackground(BLACK);

    BeginDrawing();
        std::string fpsStr = std::to_string(GetFPS());
        DrawText(fpsStr.c_str(), 10, 10, 20, WHITE);
        std::string deltaTime = std::to_string(GetFrameTime());
        DrawText(deltaTime.c_str(), 10, 30, 20, WHITE);
    EndDrawing();
}
