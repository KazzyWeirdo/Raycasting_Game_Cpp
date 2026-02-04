#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Map {
public:
    Map();

    // 0 = empty space, 1 = wall
    int getTile(int x, int y) const;

    void draw(sf::RenderWindow& window) const;

    // Getters
    int getWidth() const { return mapWidth; }
    int getHeight() const { return mapHeight; }
    int getTileSize() const { return tileSize; }
private:
    int mapWidth;
    int mapHeight;
    int tileSize;

    std::vector<int> tiles;
};