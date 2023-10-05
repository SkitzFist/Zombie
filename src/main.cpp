#include "../include/raylib.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "Game.hpp"
#include "Log.hpp"

inline Game game;

inline void run();

int main(){


    #if PLATFORM_WEB
        Log::info("Test");
       emscripten_set_main_loop(run, 0, 1);
    #else
        game.run();
        CloseWindow();
        return 0;
    #endif
}

inline void run(){
    game.webRun();    
}

inline void destroy(){
    #if PLATFORM_WEB
        emscripten_cancel_main_loop();
        CloseWindow();
    #endif
}