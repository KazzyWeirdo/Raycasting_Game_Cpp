#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Player.hpp"
#include "MapManager.hpp"

class LevelCreator {
private:
    char m_nameBuffer[128];
    bool m_placingPlayer;
    
    void handleGui(Map& map, LevelData& data, bool& dataChanged);
    void handleInput(sf::RenderWindow& window, Map& map, Player& player, LevelData& data);

public:
    LevelCreator();
    
    void update(sf::RenderWindow& window, Map& map, Player& player);
};