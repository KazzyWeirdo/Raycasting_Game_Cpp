#include <SFML/Graphics.hpp>
#include <optional>
#include "include/Constant.hpp"
#include "include/Map.hpp"
#include "include/Player.hpp"
#include "include/Raycaster.hpp"
#include "include/MapManager.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE);
    window.setFramerateLimit(Constants::FRAME_RATE);

    Map worldMap;
    Player player;

    LevelData testLevel;
    testLevel.width = 10;
    testLevel.height = 10;
    testLevel.tiles = std::vector<int>(100, 0); 
    testLevel.tiles[55] = 1;
    testLevel.skyR = 50; testLevel.skyG = 0; testLevel.skyB = 50;
    testLevel.floorR = 0; testLevel.floorG = 50; testLevel.floorB = 0;
    testLevel.fogIntensity = 3.5f;

    MapManager::ensureAssetsDirectory();
    MapManager::saveMap("prueba.map", testLevel);
    std::cout << "Mapa guardado en assets/prueba.map" << std::endl;
    LevelData loadedLevel = MapManager::loadMap("prueba");
    worldMap.loadLevel(loadedLevel);

    Raycaster raycaster;

    sf::Clock clock;

    while (window.isOpen()) {

        float dt = clock.restart().asSeconds() * 60.0f;
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        player.update(worldMap, dt);
        window.clear(sf::Color::Black);

        sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
        sky[0] = sf::Vertex{{0.0f, 0.0f}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        sky[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, 0.0f}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        sky[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        sky[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        window.draw(sky);

        sf::VertexArray floor(sf::PrimitiveType::TriangleStrip, 4);
        floor[0] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        floor[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, sf::Color(Constants::FOG_R, Constants::FOG_G, Constants::FOG_B)};
        floor[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT}, sf::Color(Constants::FLOOR_R, Constants::FLOOR_G, Constants::FLOOR_B)};
        floor[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT}, sf::Color(Constants::FLOOR_R, Constants::FLOOR_G, Constants::FLOOR_B)};
        window.draw(floor);

        raycaster.draw(window, worldMap, player);

        /* For debugging: draw 2D map and player
        
        worldMap.draw(window);
        player.draw(window);
        
        */

        window.display();
    }

    return 0;
}