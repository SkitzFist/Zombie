#ifndef _Game
#define _Game

#include "../include/raylib.h"

class Game{
public:
    Game();
    ~Game();

    void run();
    void webRun();
private:
    void handleInputSystems();
    void handleUpdateSystems(const float dt);
    void handleRenderSystems() const;
};

#endif