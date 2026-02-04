#include "../include/Map.hpp"
#include "../include/Constant.hpp"

Map::Map() {

    mapWidth = Constants::MAP_WIDTH;
    mapHeight = Constants::MAP_HEIGHT;
    tileSize = Constants::TILE_SIZE;

    tiles.resize(mapWidth * mapHeight, 0); // Initialize all tiles to empty space
    skyColor = sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B);
    floorColor = sf::Color(Constants::FLOOR_R, Constants::FLOOR_G, Constants::FLOOR_B);
    fogIntensity = 5.5f;

    for(int x = 0; x < mapWidth; ++x) {
        tiles[x] = 1; // Top wall
        tiles[(mapHeight - 1) * mapWidth + x] = 1; // Bottom wall
    }

    for(int y = 0; y < mapHeight; ++y) {
        tiles[y * mapWidth] = 1; // Left wall
        tiles[y * mapWidth + (mapWidth - 1)] = 1; // Right wall
    }

    setTile(3, 3, 1);
}

void Map::loadLevel(const LevelData& levelData)
{
    mapWidth = levelData.width;
    mapHeight = levelData.height;
    tiles = levelData.tiles;
    skyColor = sf::Color(levelData.skyR, levelData.skyG, levelData.skyB);
    floorColor = sf::Color(levelData.floorR, levelData.floorG, levelData.floorB);
    wallColor = sf::Color(levelData.wallR, levelData.wallG, levelData.wallB);
    fogIntensity = levelData.fogIntensity;
}

int Map::getTile(int x, int y) const
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return 1; // Out of bounds is treated as wall
    }
    return tiles[y * mapWidth + x];
}

void Map::setTile(int x, int y, int value)
{
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        tiles[y * mapWidth + x] = value;
    }
}

void Map::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape tileShape(sf::Vector2f(Constants::TILE_SIZE - 1, Constants::TILE_SIZE - 1));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tile = getTile(x, y);
            if (tile == 1) {
                tileShape.setFillColor(sf::Color::White);
            } else {
                tileShape.setFillColor(sf::Color::Black);
            }
            tileShape.setPosition(sf::Vector2f(x * Constants::TILE_SIZE, y * Constants::TILE_SIZE));
            window.draw(tileShape);
        }
    }
}