cmake_minimum_required(VERSION 3.10)
project(recept CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(ZombieGame)

# Adding Raylib
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

if(EMSCRIPTEN)
  set(PLATFORM Web CACHE STRING "" FORCE)
else()
  set(PLATFORM Desktop CACHE STRING "" FORCE)
endif()

FetchContent_Declare(
  raylib 
  GIT_REPOSITORY https://github.com/raysan5/raylib.git 
  GIT_TAG master)
FetchContent_MakeAvailable(raylib)

# Adding EnTT
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG master # or a specific version tag like v3.8.0
)
FetchContent_MakeAvailable(entt)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Define the executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Link against raylib
target_link_libraries(${PROJECT_NAME} raylib)
# Link against ENTT
target_link_libraries(${PROJECT_NAME} EnTT::EnTT)

# Specify C and C++ compilers for the desktop target
if(NOT EMSCRIPTEN)
  set(CMAKE_SYSTEM_NAME Windows)
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER g++)
  set(CMAKE_MAKE_PROGRAM mingw32-make)
endif()

# Platform-specific options
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".html"
    LINK_FLAGS "-s USE_GLFW=3 -s -sFULL_ES2 -sFORCE_FILESYSTEM -sALLOW_MEMORY_GROWTH -DPLATFORM_WEB --shell-file ${CMAKE_SOURCE_DIR}/shell.html"
  )
else()
  # Compiler options for desktop platform
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /O2)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-lwinmm -lgdi32 -luser32"
)
  endif()
endif()

# Set Visual Studio as the preferred IDE for CMake
set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)

#Build for desktop
#cmake -S . -B build -G "MinGW Makefiles"
#cmake --build build
#.\build\ZombieGame.exe

#Build for web
#emcmake cmake -S . -B build_web
#cmake --build build_web
#emrun .\build_web\ZombieGame.html 
