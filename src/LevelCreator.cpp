#include "../include/LevelCreator.hpp"
#include "../include/Constant.hpp"
#include <imgui.h>
#include <cstring>
#include <iostream>
#include <algorithm>

LevelCreator::LevelCreator() : m_placingPlayer(false) {
    std::memset(m_nameBuffer, 0, sizeof(m_nameBuffer));
}

void LevelCreator::update(sf::RenderWindow& window, Map& map, Player& player) {
    LevelData levelData = map.getLevelData();
    levelData.playerStartX = player.getPosition().x;
    levelData.playerStartY = player.getPosition().y;
    bool dataChanged = false;

    handleGui(map, levelData, dataChanged);

    handleInput(window, map, player, levelData);

    if (dataChanged) {
        map.loadLevel(levelData);
    }
}

void LevelCreator::handleGui(Map& map, LevelData& levelData, bool& dataChanged) {
    ImGui::Begin("Level Settings");

    if (ImGui::IsWindowAppearing()) {
        #ifdef _WIN32 
            strcpy_s(m_nameBuffer, levelData.name.c_str());
        #else
            strncpy(m_nameBuffer, levelData.name.c_str(), sizeof(m_nameBuffer));
        #endif
    }

    if (ImGui::InputText("Level Name", m_nameBuffer, sizeof(m_nameBuffer))) {
        levelData.name = m_nameBuffer;
        dataChanged = true;
    }

    // --- Resize Map ---
    int newW = levelData.width;
    int newH = levelData.height;
    ImGui::Text("Map Size:");

    if (ImGui::InputInt("Width", &newW, 5, 5)) {
        newW = std::clamp(newW, 5, 50);
        if (newW != levelData.width) {
            levelData.width = newW;
            map.resizeMap(newW, levelData.height);
        }
    }

    if (ImGui::InputInt("Height", &newH, 5, 5)) {
        newH = std::clamp(newH, 5, 50);
        if (newH != levelData.height) {
            levelData.height = newH;
            map.resizeMap(levelData.width, newH);
        }
    }

    ImGui::Separator();

    // --- Player Spawn ---
    ImGui::Text("Player Position:");
    ImGui::Checkbox("Set Spawn Point", &m_placingPlayer);

    if(m_placingPlayer) {
        ImGui::TextColored(ImVec4(0,1,0,1), "Click on the map to set player spawn position");
    } else {
        ImGui::TextColored(ImVec4(1,1,1,0.5f), "Click to build walls / Right click to remove walls");
    }

    ImGui::Separator();

    // --- Colors & Visuals ---
    ImGui::Text("Visuals:");

    auto ColorEdit = [&](const char* label, uint8_t& r, uint8_t& g, uint8_t& b) {
        float color[3] = { r/255.0f, g/255.0f, b/255.0f };
        if (ImGui::ColorEdit3(label, color)) {
            r = (uint8_t)(color[0]*255);
            g = (uint8_t)(color[1]*255);
            b = (uint8_t)(color[2]*255);
            dataChanged = true;
        }
    };

    ColorEdit("Floor Color", levelData.floorR, levelData.floorG, levelData.floorB);
    ColorEdit("Sky Color", levelData.skyR, levelData.skyG, levelData.skyB);
    ColorEdit("Walls Color", levelData.wallR, levelData.wallG, levelData.wallB);

    if (ImGui::InputFloat("Fog Distance", &levelData.fogIntensity, 1, 1)) {
        levelData.fogIntensity = std::clamp(levelData.fogIntensity, 0.0f, 25.0f);
        dataChanged = true;
    }

    ImGui::Separator();

    static sf::Clock saveFeedbackTimer;

    // --- Save Button ---
    if (ImGui::Button("Save Map", ImVec2(100, 30))) {
        if (MapManager::saveMap(levelData.name, levelData)) {
            saveFeedbackTimer.restart();
        }
    }

    if (saveFeedbackTimer.getElapsedTime().asSeconds() < 2.0f) {
        ImGui::TextColored(ImVec4(0,1,0,1), ("Map " + levelData.name + " saved successfully!").c_str());
    }

    ImGui::End();
}

void LevelCreator::handleInput(sf::RenderWindow& window, Map& map, Player& player, LevelData& levelData) {
    if (ImGui::GetIO().WantCaptureMouse) return;

    // CTRL + S
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        static sf::Clock saveTimer;
        if (saveTimer.getElapsedTime().asMilliseconds() > 500) {
            MapManager::saveMap(levelData.name, levelData);
            saveTimer.restart();
        }
    }

    // Mouse Logic
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Coordinate calculations
        int localX = mousePos.x - map.getRenderOffsetX();
        int localY = mousePos.y - map.getRenderOffsetY();
        int mapX = localX / map.getTileSizeFor2DMap();
        int mapY = localY / map.getTileSizeFor2DMap();

        // Validate boundaries
        if (mapX >= 1 && mapX < map.getWidth() - 1 && mapY >= 1 && mapY < map.getHeight() - 1) {
            
            if(m_placingPlayer) {
                if (map.getTile(mapX, mapY) == 1) return; // Can't place player on a wall
                map.clearRespawnPoints();
                map.setTile(mapX, mapY, 2); 
                float realTileSize = Constants::TILE_SIZE;
    
                // Centering the spawn position
                float worldX = (mapX * realTileSize) + (realTileSize / 2.0f);
                float worldY = (mapY * realTileSize) + (realTileSize / 2.0f);

                player.setPosition(worldX, worldY);
            } else {
                int tileValue = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ? 0 : 1;
                if (map.getTile(mapX, mapY) != tileValue && map.getTile(mapX, mapY) != 2) { // Don't overwrite player spawn
                    map.setTile(mapX, mapY, tileValue);
                }
            }
        }
    }
}