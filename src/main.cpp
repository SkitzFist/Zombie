#include "../include/raylib.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "Game.hpp"
#include "tests/CircleComponentTest.hpp"
#include "Log.hpp"

inline void run();
inline void destroy();

inline Game& getGameInstance();

int main(){
    #if DEBUG
        bool circleComponentTestResult = circleComponentTests();
        if(!circleComponentTestResult)
            return -1;
    #endif

    
    #if PLATFORM_WEB
        emscripten_set_main_loop(run, 0, 1);
    #else
        int screenWidth = 1280;
        int screenHeight = 720;
        Game game(screenWidth, screenHeight, false);
        game.run();
        CloseWindow();
        return 0;
    #endif
}

inline Game& getGameInstance() {
    static Game gameInstance(800,600, false);
    return gameInstance;
}

inline void run(){
    getGameInstance().webRun();
}
inline void destroy(){
    #if PLATFORM_WEB
        emscripten_cancel_main_loop();
        CloseWindow();
    #endif
}