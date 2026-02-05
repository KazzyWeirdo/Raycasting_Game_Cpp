#include "../include/Player.hpp"
#include "../include/Constant.hpp"

Player::Player() {
    position = {300.0f, 300.0f};
    dir = {-1.0f, 0.0f};
    plane = {0.0f, 0.66f};
}

void Player::update(const Map& map, float dt) {
    float moveSpeed = Constants::PLAYER_SPEED * dt;
    float rotSpeed = Constants::PLAYER_ROTATION_SPEED * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        sf::Vector2f newPos = {position.x + dir.x * moveSpeed, position.y + dir.y * moveSpeed};
        CheckCollision(map, newPos);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        sf::Vector2f newPos = {position.x - dir.x * moveSpeed, position.y - dir.y * moveSpeed};
        CheckCollision(map, newPos);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        float oldDirX = dir.x;
        dir.x = dir.x * std::cos(rotSpeed) - dir.y * std::sin(rotSpeed);
        dir.y = oldDirX * std::sin(rotSpeed) + dir.y * std::cos(rotSpeed);
        
        float oldPlaneX = plane.x;
        plane.x = plane.x * std::cos(rotSpeed) - plane.y * std::sin(rotSpeed);
        plane.y = oldPlaneX * std::sin(rotSpeed) + plane.y * std::cos(rotSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        float oldDirX = dir.x;
        dir.x = dir.x * std::cos(-rotSpeed) - dir.y * std::sin(-rotSpeed);
        dir.y = oldDirX * std::sin(-rotSpeed) + dir.y * std::cos(-rotSpeed);
        
        float oldPlaneX = plane.x;
        plane.x = plane.x * std::cos(-rotSpeed) - plane.y * std::sin(-rotSpeed);
        plane.y = oldPlaneX * std::sin(-rotSpeed) + plane.y * std::cos(-rotSpeed);
    }
}

void Player::CheckCollision(const Map& map, sf::Vector2f newPosition) {
    int tileX = (int)(newPosition.x) / map.getTileSize();
    int tileY = (int)(position.y) / map.getTileSize();

    if (map.getTile(tileX, tileY) == 0) { // 0 = empty space
        position.x = newPosition.x;
    }

    tileX = (int)(position.x) / map.getTileSize();
    tileY = (int)(newPosition.y) / map.getTileSize();

    if (map.getTile(tileX, tileY) == 0) { // 0 = empty space
        position.y = newPosition.y;
    }
}

void Player::loadFromLevelData(const LevelData& levelData) {
    position = {levelData.playerStartX, levelData.playerStartY};
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
        sf::Vertex{position + dir * 20.0f, sf::Color::Red}
    };

    window.draw(playerShape);
    window.draw(line, 2, sf::PrimitiveType::Lines);
}