#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Player.hpp" 
#include "Button.hpp" 
#include "Raycaster.hpp"

class Renderer {
public:
    Renderer();
    void renderScene(sf::RenderWindow& window, const Player& player, const Map& map);

    void renderUI(sf::RenderWindow& window, const std::vector<Button>& buttons, 
        const Button& exitButton, const Button& creatorButton);

    void renderCreator(sf::RenderWindow& window, const Player& player, const Map& map);

    private:

    void drawSkyAndFloor(sf::RenderWindow& window, const Map& map);

    Raycaster m_raycaster;

    sf::Color m_skyColor;
};