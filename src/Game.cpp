#include "Game.hpp"

#include "World.hpp"

//debug
#include "Log.hpp"
#include "Timer.hpp"
#include <string>
/*
    Notes: use sparse set for entity component mapping, use SOA for components.
*/



Game::Game(){
    InitWindow(World::WIDTH, World::HEIGHT, "Zombie");
    SetExitKey(KEY_ESCAPE);
    
    nrOfEntities = 10000000;
    index = 0;
    indexUpdate = 0;

    #if PLATFORM_WEB
        Log::info("Platform web");
    #else
        Log::info("NOT WEB");
        entities.reserve(nrOfEntities);    
        Timer timer;
        for(unsigned int i = 0; i < nrOfEntities; ++i){
            entities.emplace_back(i);
            circleShapeComponents.insertComponent(i, WHITE);
        }
        Log::info(std::to_string(entities.size()) + " entities took: " + std::to_string(timer.getElapsed()));
        Log::info("Size of circleShapeComponents: " + std::to_string(sizeof(circleShapeComponents)));
        timer.start();
        for(unsigned int i = 0 ; i < nrOfEntities; ++i){
            if(!circleShapeComponents.hasEntity(entities[i].id)){
                Log::info("CircleComponent hasEntity failed: not all entities exists, failed on id: " + std::to_string(i));
                break;
            }
        }
        Log::info("CircleComponent hasEntity took: " + std::to_string(timer.getElapsed()));

        circleShapeComponents.removeComponent(entities[100000].id);
        circleShapeComponents.removeComponent(entities[10040].id);
        circleShapeComponents.removeComponent(entities[1005].id);
        circleShapeComponents.removeComponent(entities[109].id);
        circleShapeComponents.removeComponent(entities[10].id);
        circleShapeComponents.removeComponent(entities[10].id);
        SetTargetFPS(1);
    #endif
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
    #if PLATFORM_WEB
    if(entities.size() < nrOfEntities){
        unsigned int initialChunk = 100000;
        unsigned int chunk = index + initialChunk;
        for(;index < chunk && index < nrOfEntities; ++index){
            entities.emplace_back(Entity(index));
            circleShapeComponents.insertComponent(entities[index].id, YELLOW);
        }
        if(entities.size() == nrOfEntities){
            Log::info(std::to_string(entities.size()) + " entities finished initiating");
        }
    }
    #endif
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
