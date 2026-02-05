#include "../include/Raycaster.hpp"
#include "../include/Constant.hpp"
#include <cmath>
#include <algorithm>

void Raycaster::draw(sf::RenderWindow &window, const Map &map, const Player &player) {

    sf::VertexArray walls(sf::PrimitiveType::Lines);

    // Raycasting Loop

    for (int ray = 0; ray < Constants::WINDOW_WIDTH; ++ray) {

        // Position of where we are in camera space

        float cameraX = 2 * ray / (float)Constants::WINDOW_WIDTH - 1;

        // Ray direction

        float rayDirX = player.dir.x + player.plane.x * cameraX;
        float rayDirY = player.dir.y + player.plane.y * cameraX;

        // Map position

        int mapX = (int)player.getPosition().x / map.getTileSize();
        int mapY = (int)player.getPosition().y / map.getTileSize();

        // Length of ray from current position to next x or y-side

        float sideDistX;
        float sideDistY;

        // Length of ray from one x or y-side to next x or y-side
        float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1.0f / rayDirY);

        float perpWallDist; // Perpendicular distance to wall
        int stepX;
        int stepY;

        bool hit = false;
        int side;

        // Calculate step and initial sideDist

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.getPosition().x / map.getTileSize() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - player.getPosition().x / map.getTileSize()) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.getPosition().y / map.getTileSize() - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.getPosition().y / map.getTileSize()) * deltaDistY;
        }

        // Perform DDA

        int maxDepth = 50;
        while (!hit && maxDepth-- > 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (map.getTile(mapX, mapY) == 1) hit = true;
        }

        // Calculate distance projected on camera direction

        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        // Calculate height of line to draw on screen

        int lineHeight = (int)(Constants::WINDOW_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe

        int drawStart = -lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;
        int drawEnd = lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;

        // Clamp values

        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= Constants::WINDOW_HEIGHT) drawEnd = Constants::WINDOW_HEIGHT - 1;

        sf::Color color = map.getWallColor();
        if (side == 1) {// Darker for y-side walls
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
        }

        float fog = perpWallDist / map.getFogIntensity();

        if (fog > 1.0f) fog = 1.0f;
        if (fog < 0.0f) fog = 0.0f;

        float wallFactor = 1.0f - fog;

        sf::Color finalColor;
        finalColor.r = (std::uint8_t)(color.r * wallFactor + map.getSkyColor().r * fog);
        finalColor.g = (std::uint8_t)(color.g * wallFactor + map.getSkyColor().g * fog);
        finalColor.b = (std::uint8_t)(color.b * wallFactor + map.getSkyColor().b * fog);

        walls.append(sf::Vertex{sf::Vector2f(ray, drawStart), finalColor});
        walls.append(sf::Vertex{sf::Vector2f(ray, drawEnd), finalColor});
    }
     window.draw(walls);
}

float Raycaster::normalizeAngle(float angle) {
    if (angle < 0) {
        angle += 2 * Constants::PI;
    }
    if (angle > 2 * Constants::PI) {
        angle -= 2 * Constants::PI;
    }
    return angle;
}