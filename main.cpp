#include <SFML/Graphics.hpp>
#include <optional>
#include "include/Constant.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), Constants::WINDOW_TITLE);

    window.setFramerateLimit(Constants::FRAME_RATE);

    sf::CircleShape player(Constants::PLAYER_SIZE);
    player.setFillColor(sf::Color::Yellow);
    player.setPosition({300.f, 300.f});

    while (window.isOpen()) {
        
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
        
        window.clear(sf::Color(50, 50, 50));
        window.draw(player);
        window.display();
    }

    return 0;
}