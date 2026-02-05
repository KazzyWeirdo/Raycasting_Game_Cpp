cmake_minimum_required(VERSION 3.10)

project(RaycasterPortfolio VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler Warnings
add_compile_options(-Wall -Wextra)

# Fetch SFML and ImGui-SFML using FetchContent
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# ImGui-SFML depends on ImGui, so we need to set the IMGUI_DIR variable before fetching it
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)

# ImGui-SFML depends on ImGui, so we need to set the IMGUI_DIR variable before fetching it
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

# Grab all source files
file(GLOB SOURCES "src/*.cpp")

add_executable(Game main.cpp ${SOURCES})

target_compile_features(Game PRIVATE cxx_std_17)
target_link_libraries(Game PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)