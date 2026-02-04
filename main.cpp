#include <SFML/Graphics.hpp>
#include <optional>
#include "include/Constant.hpp"
#include "include/Map.hpp"
#include "include/Player.hpp"
#include "include/Raycaster.hpp"
#include "include/MapManager.hpp"
#include "include/Button.hpp"
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

    std::vector<Button> mapButtons;
    MapManager::ensureAssetsDirectory();
    std::vector<std::string> availableMaps= MapManager::listAvailableMaps();

    Button exitButton(600.0f, 100.0f, 400.0f, 50.0f, "Exit", font);
    Button creatorButton(600.0f, 220.0f, 400.0f, 50.0f, "Map Creator", font);

    auto refreshMenu = [&]() {
        mapButtons.clear();
        std::vector<std::string> availableMaps= MapManager::listAvailableMaps();

        float startY = 100.0f;
        for (const auto& mapName : availableMaps) {
            mapButtons.emplace_back(100.0f, startY, 400.0f, 50.0f, mapName, font);
            startY += 60.0f;
        }
    };

    refreshMenu();

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
                    creatorButton.update(sf::Mouse::getPosition(window));
                    if (creatorButton.isClicked(sf::Mouse::getPosition(window)) && event->is<sf::Event::MouseButtonPressed>()) {
                        gameStateCurrent = GameState::CREATOR;
                    }
                    break;
                case GameState::GAME:
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                        if (keyPressed->code == sf::Keyboard::Key::Escape) {
                            gameStateCurrent = GameState::MENU;
                            sf::sleep(sf::milliseconds(200)); // Prevent immediate re-entry
                        }
                    }
                    break;
                case GameState::CREATOR:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                            gameStateCurrent = GameState::MENU;
                            refreshMenu();
                            sf::sleep(sf::milliseconds(200)); // Prevent immediate re-entry
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                        LevelData dataToSave = worldMap.getLevelData();

                        if (MapManager::saveMap("custom_map.map", dataToSave)) {
                            std::cout << "Map saved as 'custom_map.map'" << std::endl;
                        }
                        sf::sleep(sf::milliseconds(200)); // Prevent multiple saves
                    }

                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                        int mapX = (int)(mousePos.x / Constants::TILE_SIZE);
                        int mapY = (int)(mousePos.y / Constants::TILE_SIZE);

                        if(mapX >= 0 && mapX < worldMap.getWidth() && 
                        mapY >= 0 && mapY < worldMap.getHeight()) {
                            int tileValue = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? 0 : 1;

                            if(worldMap.getTile(mapX, mapY) != tileValue) {
                                worldMap.setTile(mapX, mapY, tileValue);
                            }
                        }
                    }
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
                creatorButton.update((sf::Mouse::getPosition(window)));
                creatorButton.draw(window);
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
                break;
            }
            case GameState::CREATOR:
                worldMap.draw(window);
                player.draw(window);
                
                break;
        }

        window.display();
    }

    return 0;
}