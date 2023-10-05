#include "Game.hpp"

#include <chrono>
#include <string>

#include "World.hpp"

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
    auto previous = std::chrono::system_clock::now();
    while (!WindowShouldClose())
    {
        auto current = std::chrono::system_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(current - previous).count();
        previous = current;

        handleInputSystems();
        handleUpdateSystems(dt);
        handleRenderSystems();
    }    
}

void Game::handleInputSystems(){

}

void Game::handleUpdateSystems(float dt){

}

void Game::handleRenderSystems(){
    ClearBackground(BLACK);

    BeginDrawing();
        std::string fpsStr = std::to_string(GetFPS());
        DrawText(fpsStr.c_str(), 10, 10, 20, WHITE);
    EndDrawing();
}
