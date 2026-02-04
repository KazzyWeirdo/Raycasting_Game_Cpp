#include "../include/Raycaster.hpp"
#include "../include/Constant.hpp"
#include <cmath>
#include <algorithm>

void Raycaster::draw(sf::RenderWindow &window, const Map &map, const Player &player) {

    // Initial Configuration

    float rayAngle = player.getAngle() - (Constants::FOV * Constants::DR) / 2.0f;

    float angleStep = (Constants::FOV * Constants::DR) / (float)Constants::WINDOW_WIDTH;

    sf::VertexArray walls(sf::PrimitiveType::Lines);

    // Raycasting Loop

    for (int ray = 0; ray < Constants::WINDOW_WIDTH; ++ray) {

        rayAngle = normalizeAngle(rayAngle);

        // Ray direction

        float rayX = std::cos(rayAngle);
        float rayY = std::sin(rayAngle);

        // Map position

        int mapX = (int)player.getPosition().x / map.getTileSize();
        int mapY = (int)player.getPosition().y / map.getTileSize();

        // Length of ray from current position to next x or y-side

        float sideDistX;
        float sideDistY;

        // Length of ray from one x or y-side to next x or y-side
        float deltaDistX = (rayX == 0) ? 1e30 : std::abs(1.0f / rayX);
        float deltaDistY = (rayY == 0) ? 1e30 : std::abs(1.0f / rayY);

        float perpWallDist; // Perpendicular distance to wall
        int stepX;
        int stepY;

        bool hit = false;
        int side;

        if (rayX < 0) {
            stepX = -1;
            sideDistX = (player.getPosition().x / map.getTileSize() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - player.getPosition().x / map.getTileSize()) * deltaDistX;
        }

        if (rayY < 0) {
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

            if (map.getTile(mapX, mapY) > 0) hit = true;
        }

        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        // Fish eye fix
        float ca = player.getAngle() - rayAngle;
        ca = normalizeAngle(ca);
        perpWallDist = perpWallDist * std::cos(ca);

        // Calculate height of line to draw on screen

        int lineHeight = (int)(Constants::WINDOW_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe

        int drawStart = -lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;
        int drawEnd = lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;

        // Clamp values

        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= Constants::WINDOW_HEIGHT) drawEnd = Constants::WINDOW_HEIGHT - 1;

        sf::Color color = sf::Color::Red;
        if (side == 1) {
            color = sf::Color(0, 200, 0); // Darker for y-side walls
        }

        walls.append(sf::Vertex{sf::Vector2f(ray, drawStart), color});
        walls.append(sf::Vertex{sf::Vector2f(ray, drawEnd), color});

        rayAngle += angleStep;
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