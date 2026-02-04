#include "../include/Map.hpp"
#include "../include/Constant.hpp"

Map::Map() {

    mapWidth = Constants::MAP_WIDTH;
    mapHeight = Constants::MAP_HEIGHT;
    tileSize = Constants::TILE_SIZE;

    // Initialize the map layout
    tiles = {
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,0,0,0,1,
        1,0,0,0,0,0,0,1,0,0,0,1,
        1,0,0,0,0,0,0,1,0,0,0,1,
        1,0,0,0,0,0,0,1,0,0,0,1,
        1,0,1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,1,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
    };
}

int Map::getTile(int x, int y) const
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return 1; // Out of bounds is treated as wall
    }
    return tiles[y * mapWidth + x];
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