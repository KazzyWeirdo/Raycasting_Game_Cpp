#include <SFML/Graphics.hpp>
#include <optional>
#include "include/Constant.hpp"
#include "include/Map.hpp"
#include "include/Player.hpp"
#include "include/Raycaster.hpp"
#include "include/MapManager.hpp"
#include "include/MapButton.hpp"
#include <iostream>

enum class GameState {
    MENU,
    GAME,
    CREATOR
};

int main() {
    sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE);
    window.setFramerateLimit(Constants::FRAME_RATE);

    sf::Font font;
        if (!font.openFromFile("../assets/arial.ttf")) {
            std::cerr << "Error loading font 'assets/arial.ttf'" << std::endl;
            return -1;
        }

    Map worldMap;
    Player player;
    Raycaster raycaster;
    sf::Clock clock;

    // Program State
    GameState gameStateCurrent = GameState::MENU;

    std::vector<MapButton> mapButtons;
    MapManager::ensureAssetsDirectory();
    std::vector<std::string> availableMaps = MapManager::listAvailableMaps();

    float startY = 100.0f;
    for (const auto& mapName : availableMaps) {
        mapButtons.emplace_back(200.0f, startY, 400.0f, 50.0f, mapName, font);
        startY += 60.0f;
    }

    MapButton exitButton(200.0f, startY, 400.0f, 50.0f, "Exit", font);

    while (window.isOpen()) {

        float dt = clock.restart().asSeconds() * 60.0f;
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            switch(gameStateCurrent) {
                case GameState::MENU:
                    for (auto& button : mapButtons) {
                        button.update(sf::Mouse::getPosition(window));
                        if (button.isClicked(sf::Mouse::getPosition(window)) && event->is<sf::Event::MouseButtonPressed>()) {
                            std::cout << "Loading map: " << button.getText() << std::endl;
                            LevelData levelData = MapManager::loadMap(button.getText());
                            if (levelData.width > 0) {
                                worldMap.loadLevel(levelData);
                                player = Player();
                                gameStateCurrent = GameState::GAME;
                            }
                            else {
                                std::cerr << "Failed to load map: " << button.getText() << std::endl;
                            }
                        }
                    }
                    exitButton.update(sf::Mouse::getPosition(window));
                    if (exitButton.isClicked(sf::Mouse::getPosition(window)) && event->is<sf::Event::MouseButtonPressed>()) {
                        window.close();
                    }
                    break;
                case GameState::GAME:
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                        if (keyPressed->code == sf::Keyboard::Key::Escape) {
                            gameStateCurrent = GameState::MENU;
                        }
                    }
                    break;
                case GameState::CREATOR:
                    break;
            }

        }

        window.clear(worldMap.getSkyColor());
        
        switch(gameStateCurrent) {
            case GameState::MENU:
                for (auto& button : mapButtons) {
                    button.update((sf::Mouse::getPosition(window)));
                    button.draw(window);
                }
                exitButton.update((sf::Mouse::getPosition(window)));
                exitButton.draw(window);
                break;
            case GameState::GAME: {
                player.update(worldMap, dt);
                sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
                sky[0] = sf::Vertex{{0.0f, 0.0f}, worldMap.getSkyColor()};
                sky[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, 0.0f}, worldMap.getSkyColor()};
                sky[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, worldMap.getSkyColor()};
                sky[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, worldMap.getSkyColor()};
                window.draw(sky);

                sf::VertexArray floor(sf::PrimitiveType::TriangleStrip, 4);
                floor[0] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT / 2}, worldMap.getSkyColor()};
                floor[1] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT / 2}, worldMap.getSkyColor()};
                floor[2] = sf::Vertex{{0.0f, (float)Constants::WINDOW_HEIGHT}, worldMap.getFloorColor()};
                floor[3] = sf::Vertex{{(float)Constants::WINDOW_WIDTH, (float)Constants::WINDOW_HEIGHT}, worldMap.getFloorColor()};
                window.draw(floor);

                raycaster.draw(window, worldMap, player);

                /* For debugging: draw 2D map and player
                
                worldMap.draw(window);
                player.draw(window);
                
                */
                break;
            }
            case GameState::CREATOR:
                break;
        }

        window.display();
    }

    return 0;
}