#include "../include/Raycaster.hpp"
#include "../include/Constant.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

void Raycaster::draw(sf::RenderWindow &window, const Map &map, const Player &player) {

    sf::VertexArray walls(sf::PrimitiveType::Lines);

    for (int ray = 0; ray < Constants::WINDOW_WIDTH; ++ray) {

        float cameraX = 2 * ray / (float)Constants::WINDOW_WIDTH - 1;

        float rayDirX = player.dir.x + player.plane.x * cameraX;
        float rayDirY = player.dir.y + player.plane.y * cameraX;

        int mapX = (int)player.getPosition().x / map.getTileSize();
        int mapY = (int)player.getPosition().y / map.getTileSize();

        // Length of ray from current position to next x or y-side
        float sideDistX;
        float sideDistY;

        // Length of ray from one x or y-side to next x or y-side
        // These are derived from the ray direction vector and are used to calculate how much to increase sideDistX or sideDistY when we step in x or y direction
        float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1.0f / rayDirY);

        float perpWallDist;
        int stepX;
        int stepY;

        bool hit = false;
        int side; // hits North-South wall (0) or East-West wall (1)

        // We calculate the step the player takes in the grid and the initial sideDistX and sideDistY based on the ray direction. This sets us up for the DDA algorithm to efficiently step through the grid squares.
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

        // Perform DDA: jump to next map square, either in x-direction or y-direction, until we hit a wall

        int maxDepth = Constants::MAXIMUM_DEPTH;
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

        // Calculate distance projected on camera direction to avoid fish-eye effect

        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        int lineHeight = (int)(Constants::WINDOW_HEIGHT / perpWallDist);

        // Clamp values to set screen limits

        // Calculate lowest and highest pixel to fill in current stripe (centrally aligned), then we define the screen limits

        int drawStart = -lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;
        int drawEnd = lineHeight / 2 + Constants::WINDOW_HEIGHT / 2;

        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= Constants::WINDOW_HEIGHT) drawEnd = Constants::WINDOW_HEIGHT - 1;

        /*
        In order to draw textures in a wall:
        We need to calculate where the ray hits in a wall. The value of this value
        must be between 1 and 0 to determine which exact percentatge has hit:
        0.0 = left corner
        0.5 = center
        0.9 = right corner
        By multiplying wallX with the size of the texture we get the exact Xcoordinate
        We now need to inverse this coordinate in certain direction so we don't have
        mirror effect
        */

        float wallX;
        if (side == 0) wallX = (player.getPosition().y/ map.getTileSize()) + (perpWallDist * rayDirY);
        else           wallX = (player.getPosition().x / map.getTileSize()) + (perpWallDist * rayDirX);
        wallX -= floor((wallX));

        int texSize = Constants::TEXTURE_SIZE; // Supposed size of the texture, it's a test variable we simulate 64x64 size.
        int texX = static_cast<int>(wallX * static_cast<float>(texSize));

        if (side == 0 && rayDirX > 0) texX = texSize - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texSize - texX - 1;

        // --- Coloring and Fog ---

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

        color.a = 255 * wallFactor;

        /*
            In order to not get a weird visual effect where the textures
            stretch when you get close to the wall, we need to calculate
            how much we increase the coordinate per screen pixel and the 
            initial position of said texture.

            This doesn't respect the fog settings and color, getting close
            will slowly change the color of the textures. Only a darker
            fog will work as intended.
        */

        float step = 1.0f * texSize / lineHeight;
        float texPos = (drawStart - Constants::WINDOW_HEIGHT / 2 + lineHeight / 2) * step;

        walls.append(sf::Vertex{sf::Vector2f(ray, drawStart), color, sf::Vector2f(texX, texPos)});
        walls.append(sf::Vertex{sf::Vector2f(ray, drawEnd), color, sf::Vector2f(texX, texPos + (drawEnd - drawStart) * step)});
        
    }


     window.draw(walls, map.getTextureWall());
}