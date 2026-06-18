cmake_minimum_required(VERSION 3.31)
project(particles LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# ── SFML 3.1.0 ────────────────────────────────────────────────────────────────
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

# ── Dear ImGui ────────────────────────────────────────────────────────────────
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9
    GIT_SHALLOW ON)

# ── ImGui-SFML (v3 / master) ──────────────────────────────────────────────────
FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG master
    GIT_SHALLOW ON)

FetchContent_MakeAvailable(SFML imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(imgui-sfml)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(particles src/main.cpp src/sim.cpp src/grid.cpp src/utils.cpp)

target_compile_features(particles PRIVATE cxx_std_23)
target_link_libraries(particles PRIVATE
    ImGui-SFML::ImGui-SFML
    SFML::Graphics
    SFML::Window)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)