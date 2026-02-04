#include "../include/Player.hpp"
#include "../include/Constant.hpp"

Player::Player() {
    position = {300.0f, 300.0f};
    angle = 0.0f;
    direction = {1.0f, 0.0f};
}

void Player::update(const Map& map, float dt) {
    sf::Vector2f newPosition = position;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        newPosition += direction * Constants::PLAYER_SPEED * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        newPosition -= direction * Constants::PLAYER_SPEED * dt;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        angle -= Constants::PLAYER_ROTATION_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        angle += Constants::PLAYER_ROTATION_SPEED;
    }

    // Update direction based on angle
    direction.x = cos(angle);
    direction.y = sin(angle);

    // Check for collisions before updating position
    CheckCollision(map, newPosition);
}

void Player::CheckCollision(const Map& map, sf::Vector2f newPosition) {
    int tileX = (int)(newPosition.x) / map.getTileSize();
    int tileY = (int)(newPosition.y) / map.getTileSize();

    if (map.getTile(tileX, tileY) == 0) { // 0 = empty space
        position = newPosition;
    }
}

void Player::draw(sf::RenderWindow& window) const {
    sf::CircleShape playerShape(Constants::PLAYER_SIZE);
    playerShape.setFillColor(sf::Color::Yellow);
    playerShape.setOrigin(sf::Vector2f(Constants::PLAYER_SIZE, Constants::PLAYER_SIZE));
    playerShape.setPosition(position);

    // Draw direction line
    sf::Vertex line[] =
    {
        sf::Vertex{position, sf::Color::Red},
        sf::Vertex{position + direction * 20.0f, sf::Color::Red}
    };

    window.draw(playerShape);
    window.draw(line, 2, sf::PrimitiveType::Lines);
}