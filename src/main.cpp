#include "../include/raylib.h"
#ifdef PLATFORM_WEB
#include <emscripten.h>
#endif


void run(){
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }
    
}

int main(){

    InitWindow(800, 600, "test");
    SetExitKey(KEY_ESCAPE);    

    #if PLATFORM_WEB
       emscripten_set_main_loop(run, 0, 1);
    #else
        run();
        return 0;
    #endif
}