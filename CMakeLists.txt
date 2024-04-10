cmake_minimum_required(VERSION 3.10)
project(ZombieGame CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

include(FetchContent)

# Adding Raylib
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

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Define the executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

target_link_libraries(${PROJECT_NAME} raylib)

if(NOT EMSCRIPTEN)
  set(CMAKE_SYSTEM_NAME Windows)
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER g++)
  set(CMAKE_MAKE_PROGRAM mingw32-make)
endif()

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  target_compile_options(${PROJECT_NAME} PRIVATE -msimd128 -msse4.1 -O3 -pthread)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".html"
    LINK_FLAGS "-msimd128 -O3 -s USE_GLFW=3 -s -sFULL_ES2 -sALLOW_MEMORY_GROWTH -DPLATFORM_WEB -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=16 --shell-file ${CMAKE_SOURCE_DIR}/Shell.html"
  )
else()
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /O2)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -mavx2 -mfma -Wall -Wextra -O3 -msse2 -mfpmath=sse  -ftree-vectorizer-verbose=5)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-O3 -msse2 -mfpmath=sse  -ftree-vectorizer-verbose=5 -lwinmm -lgdi32 -luser32"
)
  endif()
endif()
