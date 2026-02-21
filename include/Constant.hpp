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

    // --- Map Basic Configuration ---
    constexpr char DEFAULT_LEVEL_NAME[] = "default_map";
    constexpr int DEFAULT_MAP_WIDTH = 12;
    constexpr int DEFAULT_MAP_HEIGHT = 12;
    constexpr float TILE_SIZE = 64.0f;

    constexpr float DEFAULT_FOG_DISTANCE = 5.5f;
    constexpr std::uint8_t DEFAULT_FOG_R = 150;
    constexpr std::uint8_t DEFAULT_FOG_G = 150;
    constexpr std::uint8_t DEFAULT_FOG_B = 150;

    constexpr std::uint8_t DEFAULT_FLOOR_R = 20;
    constexpr std::uint8_t DEFAULT_FLOOR_G = 50;
    constexpr std::uint8_t DEFAULT_FLOOR_B = 20;

    constexpr std::uint8_t DEFAULT_WALL_R = 100;
    constexpr std::uint8_t DEFAULT_WALL_G = 100;
    constexpr std::uint8_t DEFAULT_WALL_B = 100;

    constexpr sf::Color RESPAWN_POINT_COLOR = sf::Color::Green;

    // --- Player Configuration  ---
    constexpr float PLAYER_SPEED = 5.0f;
    constexpr float PLAYER_ROTATION_SPEED = 0.1f;
    constexpr float PLAYER_SIZE = 8.0f;

    // --- Map Button Configuration ---
    constexpr std::uint8_t IDLE_MAP_BUTTON_R = 70;
    constexpr std::uint8_t IDLE_MAP_BUTTON_G = 70;
    constexpr std::uint8_t IDLE_MAP_BUTTON_B = 70;

    constexpr std::uint8_t HOVER_MAP_BUTTON_R = 100;
    constexpr std::uint8_t HOVER_MAP_BUTTON_G = 100;
    constexpr std::uint8_t HOVER_MAP_BUTTON_B = 100;

    constexpr int SIZE_TEXT_BUTTON = 20;
    constexpr std::uint8_t COLOR_TEXT_BUTTON_R = 255;
    constexpr std::uint8_t COLOR_TEXT_BUTTON_G = 255;
    constexpr std::uint8_t COLOR_TEXT_BUTTON_B = 255;

    // --- Raycasting Configuration ---
    constexpr int FOV = 60;                         // Field of View in degrees
    constexpr int MAXIMUM_DEPTH = 50;               // Maximum jumps in DDA

    // -- Menu Configuration ---
    constexpr sf::Color MENU_BACKGROUND_COLOR = sf::Color(30, 30, 30);

}