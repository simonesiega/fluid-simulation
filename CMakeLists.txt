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
    src/app/ApplicationConfig.hpp
    src/simulation/cpu/GridIndex.hpp
    src/simulation/cpu/ScalarField.cpp
    src/simulation/cpu/ScalarField.hpp
    src/simulation/cpu/SimulationCPU.cpp
    src/simulation/cpu/SimulationCPU.hpp
    src/simulation/cpu/VectorField.cpp
    src/simulation/cpu/VectorField.hpp
    src/simulation/SimulationSettings.hpp
    src/main.cpp
)

target_compile_features(fluid-simulation PRIVATE cxx_std_20)

target_include_directories(fluid-simulation PRIVATE src)

target_link_libraries(fluid-simulation PRIVATE raylib)

add_custom_command(
    TARGET fluid-simulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:fluid-simulation>/assets/icons"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/logo-32-black.png"
            "$<TARGET_FILE_DIR:fluid-simulation>/assets/icons/logo-32-black.png"
    VERBATIM
)
