#include "Game.hpp"

#include "World.hpp"

//debug
#include "Log.hpp"
#include "Timer.hpp"
#include <string>
#include <cmath>
#include "PositionComponent.hpp"
#include "VelocityComponent.h"
#include "MathHack.hpp"

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
        windowPos.y += 50;
        SetWindowPosition(windowPos.x, windowPos.y);
    #endif

    m_world.width = screenWidth;
    m_world.height = screenHeight;
    m_world.tileSize = 40;

    SetTargetFPS(144);
    SetExitKey(KEY_ESCAPE);

    m_camera.offset = {0,0};
    m_camera.target = {0,0};
    m_camera.rotation = 0.f;
    m_camera.zoom = 1.f;

    for(int i = 0; i < 100; ++i){
        auto entity = m_registry.create();
        float posX = (float)GetRandomValue(0, m_world.width);
        float posY = (float)GetRandomValue(0, m_world.height);
        m_registry.emplace<PositionComponent>(entity, Vector2{posX, posY});
        float randomAccX = GetRandomValue(-100, 100) / 100.f;
        float randomAccY = GetRandomValue(-100, 100) / 100.f;
        m_registry.emplace<VelocityComponent>(entity, Vector2{0,0,}, Vector2{randomAccX, randomAccY});
    }
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
    (void)dt;

    updateZombies();
}

void Game::updateZombies(){
    auto view = m_registry.view<PositionComponent, VelocityComponent>();
    auto& world = m_world;
    
    view.each([world](PositionComponent& pos, VelocityComponent& v){
        //update velocity
        v.velocity.x += v.acceleration.x; 
        v.velocity.y += v.acceleration.y;

        constexpr const float MAX_SPEED = 500.f;

        MathHack::clamp(v.velocity.x, -MAX_SPEED, MAX_SPEED);
        MathHack::clamp(v.velocity.y, -MAX_SPEED, MAX_SPEED);
        
        //update position
        pos.position.x += v.velocity.x * GetFrameTime();
        pos.position.y += v.velocity.y * GetFrameTime();

        if(pos.position.x > world.width){
            pos.position.x = 0.f;
        }else if(pos.position.x < 0.f){
            pos.position.x = world.width;
        }

        if(pos.position.y > world.height){
            pos.position.y = 0.f;
        }else if(pos.position.y < 0.f){
            pos.position.y = world.height;
        }
        
        //damp acceleration
        //v.acceleration.x *= 0.99;
        //v.acceleration.y *= 0.99;
    });
}

void Game::handleRenderSystems(){
    ClearBackground(BLACK);

    BeginDrawing();
        BeginMode2D(m_camera);
            drawZombies();
            drawGrid();
        EndMode2D();

        drawUi(); // last
    EndDrawing();
}

void Game::drawZombies(){
    auto view = m_registry.view<const PositionComponent>();

    view.each([](const auto &pos) { 
        DrawCircle(pos.position.x, pos.position.y, 10.f, GREEN);
    });
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

void Game::drawUi()const{
    std::string fpsStr = std::to_string(GetFPS());
    DrawText(fpsStr.c_str(), 10, 10, 20, WHITE);
}
