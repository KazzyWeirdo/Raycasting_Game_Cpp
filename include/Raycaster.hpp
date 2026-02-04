#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Player.hpp"

class Raycaster {
    public:
    void draw(sf::RenderWindow &window, const Map &map, const Player &player);

    private:
    float normalizeAngle(float angle);
};