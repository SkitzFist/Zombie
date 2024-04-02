#include "CameraInput.hpp"
#include "MathHack.hpp"

// debug
#include "Log.hpp"

CameraInput::CameraInput() : m_speed(1000),
                             m_minSpeed(1),
                             m_maxSpeed(1000),
                             m_maxZoom(3.f),
                             m_minZoom(0.005f) {
}

void CameraInput::handleInput(Camera2D &camera) {
    handleZoom(camera);
    handleMovement(camera);
}

void CameraInput::handleZoom(Camera2D &camera) {
    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel == 0) {
        return;
    }

    // Get the current world position of the mouse before zooming
    Vector2 prevMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    float zoomStrength = camera.zoom > 0.03f ? 0.01f : 0.005f;
    float zoom = mouseWheel < 0 ? -zoomStrength : zoomStrength;
    camera.zoom += zoom;

    MathHack::clamp(camera.zoom, m_minZoom, m_maxZoom);

    moveTowardsMouse(camera);

    Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.target.x += (prevMouseWorldPos.x - newMouseWorldPos.x);
    camera.target.y += (prevMouseWorldPos.y - newMouseWorldPos.y);
}

void CameraInput::moveTowardsMouse(Camera2D &camera) {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    // Calculate the screen's center in world coordinates
    Vector2 screenCenterWorld = GetScreenToWorld2D((Vector2){GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f}, camera);

    // Adjust the camera's target
    camera.target.x = mouseWorldPos.x - (screenCenterWorld.x - camera.target.x);
    camera.target.y = mouseWorldPos.y - (screenCenterWorld.y - camera.target.y);
}

void CameraInput::handleMovement(Camera2D &camera) {
    int movementSpeed = (int)((m_speed * GetFrameTime()) / camera.zoom);
    MathHack::clamp(movementSpeed, m_minSpeed, m_maxSpeed);

    if (IsKeyDown(KEY_A)) {
        camera.target.x -= movementSpeed;
    } else if (IsKeyDown(KEY_D)) {
        camera.target.x += movementSpeed;
    }

    if (IsKeyDown(KEY_W)) {
        camera.target.y -= movementSpeed;
    } else if (IsKeyDown(KEY_S)) {
        camera.target.y += movementSpeed;
    }
}
