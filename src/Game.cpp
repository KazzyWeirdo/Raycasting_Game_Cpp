#include "../include/Game.hpp"
#include "../include/MapManager.hpp"
#include <iostream>

Game::Game() 
    : m_window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE),
      m_state(GameState::MENU),
      m_exitButton(600.0f, 100.0f, 400.0f, 50.0f, "Exit", m_font),
      m_creatorButton(600.0f, 220.0f, 400.0f, 50.0f, "Map Creator", m_font) {
    
    m_window.setFramerateLimit(Constants::FRAME_RATE);

    if (!m_font.openFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font 'assets/arial.ttf'" << std::endl;
        exit(-1);
    }

    m_exitButton = Button(600.0f, 100.0f, 400.0f, 50.0f, "Exit", m_font);
    m_creatorButton = Button(600.0f, 220.0f, 400.0f, 50.0f, "Map Creator", m_font);
    m_renderer = Renderer();

    MapManager::ensureAssetsDirectory();
    refreshLevelList();
}

void Game::run() {
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds() * 60.0f;
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        switch(m_state) {
            case GameState::MENU: 
                handleMenuInput(*event);
                break;
            case GameState::GAME:
                handleGameInput(*event);
                break;
            case GameState::CREATOR:
                handleCreatorInput(*event);
                break;
        }
    }
}

void Game::update(float dt) {
    switch(m_state) {
        case GameState::MENU:
            updateMenu();
            break;
        case GameState::GAME:
            m_player.update(m_worldMap, dt);
            break;
        case GameState::CREATOR:
            updateCreator();
            break;
    }
}

void Game::render() {
    m_window.clear(m_worldMap.getSkyColor());

    switch(m_state) {
        case GameState::MENU:
            m_renderer.renderUI(m_window, m_mapButtons, m_exitButton, m_creatorButton);
            break;
        case GameState::GAME:
            m_renderer.renderScene(m_window, m_player, m_worldMap);
            break;
        case GameState::CREATOR:
            m_renderer.renderCreator(m_window, m_player, m_worldMap);
            break;
    }

    m_window.display();
}

// --- Input Handlers ---

void Game::handleMenuInput(const sf::Event& event) {
    if (!event.is<sf::Event::MouseButtonPressed>()) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

    for (auto& button : m_mapButtons) {
        if (button.isClicked(mousePos)) {
            loadLevel(button.getText());
        } 
    }

    if (m_exitButton.isClicked(mousePos)) {
        m_window.close();
    }
    if (m_creatorButton.isClicked(mousePos)) {
        m_state = GameState::CREATOR;
    }
}

void Game::handleGameInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            m_state = GameState::MENU;
            sf::sleep(sf::milliseconds(200)); 
        }
    }
}

void Game::handleCreatorInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            m_state = GameState::MENU;
            refreshLevelList();
            sf::sleep(sf::milliseconds(200));
        }
    }
}

// --- Update Methods ---

void Game::updateMenu() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    for (auto& button : m_mapButtons) button.update(mousePos);
    m_exitButton.update(mousePos);
    m_creatorButton.update(mousePos);
}

void Game::updateCreator() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        LevelData dataToSave = m_worldMap.getLevelData();

        if (MapManager::saveMap(dataToSave.name, dataToSave)) {
            std::cout << "Map saved as '" << dataToSave.name << "'" << std::endl;
        }
        sf::sleep(sf::milliseconds(200)); 
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
        int mapX = mousePos.x / Constants::TILE_SIZE;
        int mapY = mousePos.y / Constants::TILE_SIZE;

        if (mapX >= 0 && mapX < m_worldMap.getWidth() && mapY >= 0 && mapY < m_worldMap.getHeight()) {
            int tileValue = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? 0 : 1;
            if (m_worldMap.getTile(mapX, mapY) != tileValue) {
                m_worldMap.setTile(mapX, mapY, tileValue);
            }
        }
    }
}

// --- Utility Methods ---

void Game::refreshLevelList() {
    m_mapButtons.clear();
    std::vector<std::string> availableMaps = MapManager::listAvailableMaps();

    float startY = 100.0f;
    for (const auto& mapName : availableMaps) {
        m_mapButtons.emplace_back(100.0f, startY, 400.0f, 50.0f, mapName, m_font);
        startY += 60.0f;
    }
}

void Game::loadLevel(const std::string& levelName) {
    LevelData levelData = MapManager::loadMap(levelName);
    if (levelData.width > 0) {
        m_worldMap.loadLevel(levelData);
        m_player = Player(); // Reset player
        m_state = GameState::GAME;
    } else {
        std::cerr << "Failed to load map: " << levelName << std::endl;
    }
}