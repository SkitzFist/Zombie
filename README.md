# Zombie


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
