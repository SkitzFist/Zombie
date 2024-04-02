#include "raylib.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "Game.hpp"
#include "Log.hpp"

inline void run();
inline void destroy();

inline Game& getGameInstance();

constexpr const int screenWidth = 1920;
constexpr const int screenHeight = 1080;

int main(){
    #if DEBUG
    #endif


    #if PLATFORM_WEB
        emscripten_set_main_loop(run, 0, 1);
    #else
        Game game(screenWidth, screenHeight, false);
        game.run();
        CloseWindow();
        return 0;
    #endif
}

inline Game& getGameInstance() {
    static Game gameInstance(screenWidth,screenHeight, false);
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