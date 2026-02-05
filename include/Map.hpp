#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

struct LevelData {
    std::string name;
    std::vector<int> tiles;
    int width, height;

    std::uint8_t skyR, skyG, skyB;
    std::uint8_t floorR, floorG, floorB;
    std::uint8_t wallR, wallG, wallB;
    float fogIntensity;
};

class Map {
public:
    Map();

    void loadLevel(const LevelData& levelData);

    // 0 = empty space, 1 = wall
    int getTile(int x, int y) const;
    void setTile(int x, int y, int value);

    void resizeMap(int newWidth, int newHeight);
    void draw(sf::RenderWindow& window) const;

    // Getters
    int getWidth() const { return mapWidth; }
    int getHeight() const { return mapHeight; }
    int getTileSize() const { return tileSize; }
    sf::Color getSkyColor() const { return skyColor; }
    sf::Color getFloorColor() const { return floorColor; }
    sf::Color getWallColor() const { return wallColor; }
    float getFogIntensity() const { return fogIntensity; }
    LevelData getLevelData() const {
        LevelData data;
        data.name = levelName;
        data.width = mapWidth;
        data.height = mapHeight;
        data.tiles = tiles;
        data.skyR = skyColor.r;
        data.skyG = skyColor.g;
        data.skyB = skyColor.b;
        data.floorR = floorColor.r;
        data.floorG = floorColor.g;
        data.floorB = floorColor.b;
        data.wallR = wallColor.r;
        data.wallG = wallColor.g;
        data.wallB = wallColor.b;
        data.fogIntensity = fogIntensity;
        return data;
    }
private:
    std::string levelName;
    int mapWidth;
    int mapHeight;
    int tileSize;

    std::vector<int> tiles;
    sf::Color skyColor;
    sf::Color floorColor;
    sf::Color wallColor;
    float fogIntensity;
};