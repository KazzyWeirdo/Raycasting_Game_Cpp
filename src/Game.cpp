#include "../include/Game.hpp"
#include "../include/MapManager.hpp"
#include <imgui.h>
#include <imgui-sfml.h>
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
    ImGui::SFML::Init(m_window);
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
            m_renderer.renderCreator(m_window, m_player, m_worldMap);
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
    LevelData levelData = m_worldMap.getLevelData();
    bool dataChanged = false;

    ImGui::Begin("Level Settings");

    static char nameBuffer[128];
    if (ImGui::IsWindowAppearing()) strcpy_s(nameBuffer, levelData.name.c_str());
    if (ImGui::InputText("Level Name", nameBuffer, sizeof(nameBuffer))) {
        levelData.name = nameBuffer;
        dataChanged = true;
    }

    int newW = levelData.width;
    int newH = levelData.height;
    ImGui::Text("Map Size:");
    if (ImGui::InputInt("Width", &newW, 5, 5)) m_worldMap.resizeMap(newW, levelData.height);
    if (ImGui::InputInt("Height", &newH, 5, 5)) m_worldMap.resizeMap(levelData.width, newH);

    ImGui::Separator();

    ImGui::Text("Player Position:");

    static bool placingPlayer = false;
    ImGui::Checkbox("Set Spawn Point", &placingPlayer);

    if(placingPlayer) {
        ImGui::TextColored(ImVec4(0,1,0,1), "Click on the map to set player spawn position");
    } else {
        ImGui::TextColored(ImVec4(1,1,1,0.5f), "Click to build walls / Right click to remove walls");
    }

    ImGui::Separator();

    ImGui::Text("Visuals:");

    float floorColor[3] = {levelData.floorR/255.0f, levelData.floorG/255.0f, levelData.floorB/255.0f};
    if (ImGui::ColorEdit3("Floor Color", floorColor)) {
        levelData.floorR = (std::uint8_t)(floorColor[0]*255);
        levelData.floorG = (std::uint8_t)(floorColor[1]*255);
        levelData.floorB = (std::uint8_t)(floorColor[2]*255);
        dataChanged = true;
    }

    float skyColor[3] = {levelData.skyR/255.0f, levelData.skyG/255.0f, levelData.skyB/255.0f};
    if (ImGui::ColorEdit3("Sky Color", skyColor)) {
        levelData.skyR = (std::uint8_t)(skyColor[0]*255);
        levelData.skyG = (std::uint8_t)(skyColor[1]*255);
        levelData.skyB = (std::uint8_t)(skyColor[2]*255);
        dataChanged = true;
    }

    float wallsColor[3] = {levelData.wallR/255.0f, levelData.wallG/255.0f, levelData.wallB/255.0f};
    if (ImGui::ColorEdit3("Walls Color", wallsColor)) {
        levelData.wallR = (std::uint8_t)(wallsColor[0]*255);
        levelData.wallG = (std::uint8_t)(wallsColor[1]*255);
        levelData.wallB = (std::uint8_t)(wallsColor[2]*255);
        dataChanged = true;
    }

    float newIntensity = levelData.fogIntensity;
    if (ImGui::InputFloat("Fog Distance", &newIntensity, 1, 1)) {
        levelData.fogIntensity = newIntensity;
        dataChanged = true;
    }

    ImGui::Separator();

    if (dataChanged) {
        m_worldMap.loadLevel(levelData);
        m_player.loadFromLevelData(levelData);
    }

    if (ImGui::Button("Save Map", ImVec2(100, 30))) {
        std::cout << levelData.playerStartX << std::endl;
        if (MapManager::saveMap(levelData.name, levelData)) {
            std::cout << "Map saved as '" << levelData.name << "'" << std::endl;
        }
         sf::sleep(sf::milliseconds(200));
    }

    ImGui::End();

    if (!ImGui::GetIO().WantCaptureMouse) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {

            if (MapManager::saveMap(levelData.name, levelData)) {
                std::cout << "Map saved as '" << levelData.name << "'" << std::endl;
            }
            sf::sleep(sf::milliseconds(200)); 
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

            int localX = mousePos.x - m_worldMap.getRenderOffsetX();
            int localY = mousePos.y - m_worldMap.getRenderOffsetY();
            
            int mapX = localX / m_worldMap.getTileSizeFor2DMap();
            int mapY = localY / m_worldMap.getTileSizeFor2DMap();

            if (mapX >= 0 && mapX < m_worldMap.getWidth() && mapY >= 0 && mapY < m_worldMap.getHeight()) {
                if(placingPlayer) {
                    m_worldMap.clearRespawnPoints();
                    m_worldMap.setTile(mapX, mapY, 2); // Mark position as player spawn point
                    levelData.playerStartX = mapX;
                    levelData.playerStartY = mapY;

                } else {
                    int tileValue = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? 0 : 1;
                    if (m_worldMap.getTile(mapX, mapY) != tileValue) {
                        m_worldMap.setTile(mapX, mapY, tileValue);
                    }
                }
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