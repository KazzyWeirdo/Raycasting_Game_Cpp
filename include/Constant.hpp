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

    // --- Player Configuration  ---
    constexpr float PLAYER_SPEED = 5.0f;
    constexpr float PLAYER_ROTATION_SPEED = 0.1f;
    constexpr float PLAYER_SIZE = 8.0f;

    // --- Raycasting Configuration ---
    constexpr int FOV = 60;                         // Field of View in degrees

}