#include "../include/Game.hpp"
#include "../include/MapManager.hpp"
#include <imgui-sfml.h>
#include <iostream>

Game::Game() 
    : m_window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close),
      m_state(GameState::MENU),
      m_exitButton(600.0f, 100.0f, 400.0f, 50.0f, "Exit", m_font),
      m_creatorButton(600.0f, 220.0f, 400.0f, 50.0f, "Map Creator", m_font) {
    
    m_window.setFramerateLimit(Constants::FRAME_RATE);

    if (!m_font.openFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Error loading font 'assets/fonts/arial.ttf'" << std::endl;
        exit(-1);
    }

    m_exitButton = Button(600.0f, 100.0f, 400.0f, 50.0f, "Exit", m_font);
    m_creatorButton = Button(600.0f, 220.0f, 400.0f, 50.0f, "Map Creator", m_font);
    m_renderer = Renderer();

    MapManager::ensureAssetsDirectory();
    refreshLevelList();
    (void)ImGui::SFML::Init(m_window);
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
        ImGui::SFML::ProcessEvent(m_window, *event);
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
    ImGui::SFML::Update(m_window, m_clock.restart());
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
            m_renderer.renderCreator(m_window, m_worldMap);
            break;
    }
    ImGui::SFML::Render(m_window);
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
        m_worldMap = Map(); // Reset map to avoid conflicts with creator
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
    m_levelCreator.update(m_window, m_worldMap);
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