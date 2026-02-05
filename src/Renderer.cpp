#include "../include/Renderer.hpp"
#include "../include/Constant.hpp"

Renderer::Renderer() {}

void Renderer::renderScene(sf::RenderWindow& window, const Player& player, const Map& map) {
    drawSkyAndFloor(window, map);
    m_raycaster.draw(window, map, player);
}

void Renderer::renderUI(sf::RenderWindow& window, const std::vector<Button>& buttons, 
    const Button& exitButton, const Button& creatorButton) {
    for (auto& button : buttons) button.draw(window);
    exitButton.draw(window);
    creatorButton.draw(window);
}

void Renderer::renderCreator(sf::RenderWindow& window, const Player& player, const Map& map) {
    map.draw(window);
    player.draw(window);
}

void Renderer::drawSkyAndFloor(sf::RenderWindow& window, const Map& m_worldMap) {
    // Dibujamos cielo y suelo optimizado
    sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
    sky[0] = sf::Vertex{{0.0f, 0.0f}, m_worldMap.getSkyColor()};
    sky[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, 0.0f}, m_worldMap.getSkyColor()};
    sky[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, m_worldMap.getSkyColor()};
    sky[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, m_worldMap.getSkyColor()};
    window.draw(sky);

    sf::VertexArray floor(sf::PrimitiveType::TriangleStrip, 4);
    floor[0] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, m_worldMap.getSkyColor()}; // Horizonte difuminado
    floor[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, m_worldMap.getSkyColor()};
    floor[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT}, m_worldMap.getFloorColor()};
    floor[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT}, m_worldMap.getFloorColor()};
    window.draw(floor);
}