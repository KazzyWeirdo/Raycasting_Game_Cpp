#include "../include/Map.hpp"
#include "../include/Constant.hpp"

Map::Map() {

    levelName = Constants::DEFAULT_LEVEL_NAME;
    mapWidth = Constants::DEFAULT_MAP_WIDTH;
    mapHeight = Constants::DEFAULT_MAP_HEIGHT;
    tileSize = Constants::TILE_SIZE;

    tileSizeFor2DMap = 0.0f;
    renderOffsetX = 0.0f;
    renderOffsetY = 0.0f;

    tiles.resize(mapWidth * mapHeight, 0); // Initialize all tiles to empty space
    skyColor = sf::Color(Constants::DEFAULT_FOG_R, Constants::DEFAULT_FOG_G, Constants::DEFAULT_FOG_B);
    floorColor = sf::Color(Constants::DEFAULT_FLOOR_R, Constants::DEFAULT_FLOOR_G, Constants::DEFAULT_FLOOR_B);
    wallColor = sf::Color(Constants::DEFAULT_WALL_R, Constants::DEFAULT_WALL_G, Constants::DEFAULT_WALL_B);
    fogIntensity = Constants::DEFAULT_FOG_DISTANCE;
    drawLimits();
    calculateOffset();
}

void Map::loadLevel(const LevelData& levelData)
{
    levelName = levelData.name;
    mapWidth = levelData.width;
    mapHeight = levelData.height;
    tiles = levelData.tiles;
    skyColor = sf::Color(levelData.skyR, levelData.skyG, levelData.skyB);
    floorColor = sf::Color(levelData.floorR, levelData.floorG, levelData.floorB);
    wallColor = sf::Color(levelData.wallR, levelData.wallG, levelData.wallB);
    fogIntensity = levelData.fogIntensity;
    drawLimits();
}

void Map::resizeMap(int newWidth, int newHeight)
{
    std::vector<int> newTiles(newWidth * newHeight, 0);

    for (int y = 0; y < std::min(mapHeight, newHeight); ++y) {
        for (int x = 0; x < std::min(mapWidth, newWidth); ++x) {
            newTiles[y * newWidth + x] = 0;
        }
    }

    mapWidth = newWidth;
    mapHeight = newHeight;
    tiles = std::move(newTiles);
    calculateOffset();
    drawLimits();
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

void Map::drawLimits()
{
    for(int x = 0; x < mapWidth; ++x) {
        tiles[x] = 1; // Top wall
        tiles[(mapHeight - 1) * mapWidth + x] = 1; // Bottom wall
    }

    for(int y = 0; y < mapHeight; ++y) {
        tiles[y * mapWidth] = 1; // Left wall
        tiles[y * mapWidth + (mapWidth - 1)] = 1; // Right wall
    }
}

void Map::clearRespawnPoints() {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (tiles[y * mapWidth + x] == 2) {
                tiles[y * mapWidth + x] = 0;
            }
        }
    }
}

void Map::calculateOffset()
{
    float tileSizeW = Constants::WINDOW_WIDTH / mapWidth;
    float tileSizeH = Constants::WINDOW_HEIGHT / mapHeight;

    tileSizeFor2DMap = std::min(tileSizeW, tileSizeH);
    float totalMapW = mapWidth * tileSizeFor2DMap;
    float totalMapH = mapHeight * tileSizeFor2DMap;

    renderOffsetX = (Constants::WINDOW_WIDTH - totalMapW) / 2.0f;
    renderOffsetY = (Constants::WINDOW_HEIGHT - totalMapH) / 2.0f;
}

void Map::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape tileShape(sf::Vector2f(tileSizeFor2DMap - 1, tileSizeFor2DMap - 1));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tile = getTile(x, y);
            if (tile == 1) {
                tileShape.setFillColor(wallColor);
                tileShape.setOutlineThickness(0.0f);
            } else if (tile == 2) {
                tileShape.setOutlineColor(Constants::RESPAWN_POINT_COLOR);
                tileShape.setOutlineThickness(-1.0f);
                tileShape.setFillColor(floorColor);
            } else {
                tileShape.setFillColor(floorColor);
                tileShape.setOutlineThickness(0.0f);
            }
            tileShape.setPosition(sf::Vector2f((x * tileSizeFor2DMap) + renderOffsetX, (y * tileSizeFor2DMap) + renderOffsetY));
            window.draw(tileShape);
        }
    }
}