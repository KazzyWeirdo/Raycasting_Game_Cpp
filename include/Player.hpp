#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Player {
public:
    Player();

    void update(const Map& map, float dt);

    void draw(sf::RenderWindow& window) const;

    // Getters
    sf::Vector2f getPosition() const { return position; }
    float getAngle() const { return angle; }
private:

    sf::Vector2f position;
    sf::Vector2f direction;
    float angle; // in radians

    void CheckCollision(const Map& map, sf::Vector2f newPosition);
};