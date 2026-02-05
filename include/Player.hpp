#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Player {
public:
    Player();

    void update(const Map& map, float dt);

    void draw(sf::RenderWindow& window, float offsetX, float offsetY, float scaleFactor) const;

    void loadFromLevelData(const LevelData& levelData);

    // Getters
    sf::Vector2f getPosition() const { return position; }

    // For raycasting
    sf::Vector2f dir;
    sf::Vector2f plane;
private:

    sf::Vector2f position;

    void CheckCollision(const Map& map, sf::Vector2f newPosition);
};