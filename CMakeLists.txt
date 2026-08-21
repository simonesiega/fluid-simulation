cmake_minimum_required(VERSION 3.20)

project(fluid-simulation LANGUAGES CXX)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(fluid-simulation
    src/main.cpp
)

target_compile_features(fluid-simulation PRIVATE cxx_std_20)

target_link_libraries(fluid-simulation PRIVATE raylib)