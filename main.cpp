#include <SFML/Graphics.hpp>
#include <optional>
#include "include/Constant.hpp"
#include "include/Map.hpp"
#include "include/Player.hpp"
#include "include/Raycaster.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE);
    window.setFramerateLimit(Constants::FRAME_RATE);

    Map worldMap;
    Player player;
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