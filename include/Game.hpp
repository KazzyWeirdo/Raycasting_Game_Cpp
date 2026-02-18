#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constant.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Raycaster.hpp"
#include "Button.hpp"
#include "Renderer.hpp"
#include "LevelCreator.hpp"

enum class GameState {
    MENU,
    GAME,
    CREATOR
};

class Game {
    public: 
        Game();
        void run();

    private:
        void processEvents();
        void update(float dt);
        void render();

        void handleMenuInput(const sf::Event& event);
        void handleCreatorInput(const sf::Event& event);
        void handleGameInput(const sf::Event& event);

        void updateMenu();
        void updateCreator();
        
        void renderMenu();
        void renderGame();
        void renderCreator();
        void drawSkyAndFloor();

        void refreshLevelList();
        void loadLevel(const std::string& levelName);

        sf::RenderWindow m_window;
        sf::Font m_font;
        sf::Clock m_clock;
        sf::Texture m_wall_Texture;
        GameState m_state;

        Map m_worldMap;
        Player m_player;
        LevelCreator m_levelCreator;

        std::vector<Button> m_mapButtons;
        Button m_exitButton;
        Button m_creatorButton;

        Renderer m_renderer;
};