#include "../include/raylib.h"

#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif

#include "Game.hpp"

inline Game game;

inline void run();

int main(){


    #if PLATFORM_WEB
       emscripten_set_main_loop(run, 0, 1);
    #else
        game.run();
        CloseWindow();
        return 0;
    #endif
}

inline void run(){
    game.webRun();
    destroy();
}

inline void destroy(){
    #if PLATFORM_WEB
        emscripten_cancel_main_loop();
        CloseWindow();
    #endif
}