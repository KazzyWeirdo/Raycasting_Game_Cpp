cmake_minimum_required(VERSION 3.10)

project(RaycasterPortfolio VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler Warnings
add_compile_options(-Wall -Wextra)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(Game main.cpp src/Map.cpp src/Player.cpp src/Raycaster.cpp src/MapManager.cpp src/Button.cpp)

target_link_libraries(Game PRIVATE SFML::Graphics SFML::Window SFML::System)