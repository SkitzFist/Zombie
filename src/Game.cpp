#include "Game.hpp"

#include "World.hpp"
#include "ComponentTypes.h"
#include "Entity.h"
#include "CollisionDetection.h"

//debug
#include "Log.hpp"
#include "Timer.hpp"
#include <string>
#include <cmath>
#include "MathHack.hpp"

inline constexpr const int MAX_ENTITIES = 100;

Rectangle a1 {0,0, 30, 30};

inline constexpr const float ZOMBIE_RADIUS = 15;
inline constexpr const float WORLD_SCALE = 1;

Game::Game(int screenWidth, int screenHeight, bool isFullscreen): 
    m_camera(), 
    m_tree(0, {0,0,screenWidth * WORLD_SCALE, screenHeight * WORLD_SCALE}){

    #if PLATFORM_WEB
        InitWindow(screenWidth, screenHeight, "Zombie");
    #else
        InitWindow(0,0, "Zombie");
        SetWindowSize(screenWidth, screenHeight);
        if(isFullscreen){
            ToggleFullscreen();
        }

        Vector2 windowPos = GetWindowPosition();
        windowPos.y += 50;
        SetWindowPosition(windowPos.x, windowPos.y);
    #endif

    m_world.width = screenWidth  * WORLD_SCALE;
    m_world.height = screenHeight * WORLD_SCALE;
    m_world.tileSize = 32;

    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    m_camera.offset = {0,0};
    m_camera.target = {0,0};
    m_camera.rotation = 0.f;
    m_camera.zoom = 1.f;
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

    Vector2 mousePos = GetMousePosition();

    a1.x = mousePos.x;
    a1.y = mousePos.y;
}

void Game::handleUpdateSystems(float dt){
    (void)dt;
    
    if(currentIndex == MAX_ENTITIES){
        return;
    }

    int maxBatch = currentIndex + maxBatchSize;
    if(maxBatch >= MAX_ENTITIES){
        maxBatch = MAX_ENTITIES - 1;
    }
    for(; currentIndex < maxBatch; ++currentIndex){
        Entity entity(currentIndex);
        
        entity.addComponent(ComponentTypes::POSITION);
        float randX = GetRandomValue(0, m_world.width);
        float randY = GetRandomValue(0, m_world.height);
        positionComponent.add(randX, randY, entity.id);
        m_tree.add(entity, {
            randX - ZOMBIE_RADIUS, randY - ZOMBIE_RADIUS,
            ZOMBIE_RADIUS * 2, ZOMBIE_RADIUS * 2
        });
        //m_entities.push_back(entity);
    }
}


Rectangle Game::getCameraRect()const{
    Vector2 cameraPos = m_camera.target; //GetScreenToWorld2D(m_camera.target, m_camera);
    Vector2 size = {GetScreenWidth() / m_camera.zoom, GetScreenHeight() / m_camera.zoom};
    return {cameraPos.x, cameraPos.y, size.x, size.y};
}

void Game::handleRenderSystems(){
    ClearBackground(BLACK);

    BeginDrawing();
        BeginMode2D(m_camera);
            drawGrid();
            drawZombie();

            //camera test
            //Rectangle cameraRect = getCameraRect();
            //DrawRectangleLines(cameraRect.x, cameraRect.y, cameraRect.width, cameraRect.height, YELLOW);
        EndMode2D();
        
        drawUi(); // last
        DrawRectangleLines(a1.x, a1.y, a1.width, a1.height, YELLOW);
    EndDrawing();
}

void Game::drawGrid()const{
    
    Color color = RAYWHITE;
    color.a = 50;

    for(int i = 0; i <= m_world.width; i += m_world.tileSize){
        DrawLine(i, 0, i, m_world.height, color);
    }

    for(int i = 0; i <= m_world.height; i += m_world.tileSize){
        DrawLine(0, i, m_world.width, i, color);
    }
}

int drawnCount = 0;
void Game::drawUi()const{
    std::string fpsStr = std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), 10, 40, 40, WHITE);

    std::string countStr = "Entities drawn: " + std::to_string(drawnCount);
    DrawText(countStr.c_str(), 10, 80, 40, WHITE);

    std::string maxEnt = "Total entities: " + std::to_string(currentIndex);
    DrawText(maxEnt.c_str(), 10, 120, 40, WHITE);
}

void Game::drawZombie() const{
    Rectangle cameraRect = getCameraRect();
    drawnCount = 0;
    for(const Entity& entity : m_tree.search(a1)){
        if(entity.hasComponent(ComponentTypes::POSITION)){
            Vector2 pos = positionComponent.getPositionByIndex(entity.id);
            DrawCircle(pos.x, pos.y, ZOMBIE_RADIUS, GREEN);
            ++drawnCount;
        }
    }
    /*
    for(const Entity& entity : m_entities){
        if(entity.hasComponent(ComponentTypes::POSITION)){
            Vector2 position = positionComponent.getPositionByIndex(entity.id);
            if(position.x > cameraRect.x && position.x < (cameraRect.x + cameraRect.width) &&
               position.y > cameraRect.y && position.y < (cameraRect.y + cameraRect.height)){
                DrawCircle(position.x, position.y, 15.f, GREEN);
                ++drawnCount;
            }
        }
    }
    */
}
