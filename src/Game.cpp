#include "Game.hpp"

#include "World.hpp"

//debug
#include "Log.hpp"
#include "Timer.hpp"
#include <string>
/*
    Notes: use sparse set for entity component mapping, use SOA for components.
*/



Game::Game(int screenWidth, int screenHeight, bool isFullscreen){
    #if PLATFORM_WEB
        InitWindow(screenWidth, screenHeight, "Zombie");
    #else
        InitWindow(0,0, "Zombie");
        SetWindowSize(screenWidth, screenHeight);
        if(isFullscreen){
            ToggleFullscreen();
        }
    #endif

    SetExitKey(KEY_ESCAPE);
    
    nrOfEntities = 10000000;
    indexUpdate = 0;
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
    if(entities.size() < nrOfEntities){
        initiateEntities();
        return;
    }
}

void Game::handleRenderSystems(){
    ClearBackground(BLACK);

    BeginDrawing();
        drawUi(); // last
    EndDrawing();
}

void Game::initiateEntities(){
        unsigned int chunkSize = 1000000;
        unsigned int chunk = indexUpdate + chunkSize;
        for(;indexUpdate < chunk && indexUpdate < nrOfEntities; ++indexUpdate){
            entities.emplace_back(Entity(indexUpdate));
            circleShapeComponents.insertComponent(entities[indexUpdate].id, YELLOW);
        }
        if(entities.size() == nrOfEntities){
            Log::info(std::to_string(entities.size()) + " entities finished initiating");
        }
}


void Game::drawUi(){
    std::string fpsStr = std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), 10, 10, 20, WHITE);
}

