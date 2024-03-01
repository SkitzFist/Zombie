#include "raylib.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "Game.hpp"
#include "Log.hpp"

inline void run();
inline void destroy();

inline Game& getGameInstance();

int main(){
    #if DEBUG
    #endif

    
    #if PLATFORM_WEB
        emscripten_set_main_loop(run, 0, 1);
    #else
        int screenWidth = 1920 * 2;
        int screenHeight = 1280 * 2;
        Game game(screenWidth, screenHeight, false);
        game.run();
        CloseWindow();
        return 0;
    #endif
}

inline Game& getGameInstance() {
    constexpr const int screenWidth = 800;
    constexpr const int screenHeight = 600;
    static Game gameInstance(screenWidth,screenHeight, false);
    return gameInstance;
}

inline void run(){
    getGameInstance().webRun();
    destroy();
}
inline void destroy(){
    #if PLATFORM_WEB
        emscripten_cancel_main_loop();
        CloseWindow();
    #endif
}