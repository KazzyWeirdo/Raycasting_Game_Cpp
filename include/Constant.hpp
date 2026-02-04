#pragma once
#include <cmath>

namespace Constants {
    // --- Window Configuration ---
    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 512;
    constexpr int FRAME_RATE = 60;
    constexpr char WINDOW_TITLE[] = "Raycasting Project";

    // --- Mathematical constants ---
    constexpr double PI = 3.1415926535;
    constexpr double P2 = PI / 2;
    constexpr double P3 = 3 * PI / 2;
    constexpr double DR = 0.0174533;                // one degree in radians

    // --- Map Configuration ---
    constexpr int MAP_WIDTH = 12;
    constexpr int MAP_HEIGHT = 12;
    constexpr float TILE_SIZE = 144.0f;

    constexpr float FOG_DISTANCE = 7.5f;
    constexpr std::uint8_t FOG_R = 150;
    constexpr std::uint8_t FOG_G = 150;
    constexpr std::uint8_t FOG_B = 150;

    constexpr std::uint8_t FLOOR_R = 20;
    constexpr std::uint8_t FLOOR_G = 50;
    constexpr std::uint8_t FLOOR_B = 20;

    // --- Player Configuration  ---
    constexpr float PLAYER_SPEED = 5.0f;
    constexpr float PLAYER_ROTATION_SPEED = 0.1f;
    constexpr float PLAYER_SIZE = 8.0f;

    // --- Raycasting Configuration ---
    constexpr int FOV = 60;                         // Field of View in degrees

}