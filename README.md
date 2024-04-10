# Zombie

As github does not easily let me change Cross-Origin-Embedder-Policy and Cross-Origin-Opener-Policy,
and that inherntly disables SharedArrayBuffers which the project is dependent on. I had to host the simulation
on itch.io : https://skitzfist.itch.io/web-boids

# BUILD

## Build and run for desktop (windows)
Stand in the root directory, this will create a "build" folder.  
cmake -S . -B build -G "MinGW Makefiles"   
cmake --build build  
.\build\ZombieGame.exe  

## Build and run for web
Requires emscripten preconfigured  
Stand in the root directory, this will create a "build_web" folder.  
emcmake cmake -S . -B build_web  
cmake --build build_web  
emrun .\build_web\ZombieGame.html   
