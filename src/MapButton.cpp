#include "../include/MapButton.hpp"
#include "../include/Constant.hpp"

MapButton::MapButton(float x, float y, float width, float height, 
                     const std::string& text, const sf::Font& font)
                     // SFML 3 requires initializing sf::Text with a font before setting other properties
                     : buttonText(font) {

    this->mapName = text;
    idleColor = sf::Color(Constants::IDLE_MAP_BUTTON_R, Constants::IDLE_MAP_BUTTON_G, Constants::IDLE_MAP_BUTTON_B);
    hoverColor = sf::Color(Constants::HOVER_MAP_BUTTON_R, Constants::HOVER_MAP_BUTTON_G, Constants::HOVER_MAP_BUTTON_B);

    buttonShape.setPosition({x, y});
    buttonShape.setSize({width, height});
    buttonShape.setFillColor(idleColor);

    buttonText.setString(text);
    buttonText.setCharacterSize(Constants::SIZE_TEXT_BUTTON);
    buttonText.setFillColor(sf::Color(Constants::COLOR_TEXT_BUTTON_R, Constants::COLOR_TEXT_BUTTON_G, Constants::COLOR_TEXT_BUTTON_B));
    
    // Center the text within the button
    centerText();
    buttonText.setPosition({x + width / 2.0f, y + height / 2.0f});
}

void MapButton::draw(sf::RenderWindow& window) const {
    window.draw(buttonShape);
    window.draw(buttonText);
}

void MapButton::update(sf::Vector2i mousePosition) {
    if (buttonShape.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
        buttonShape.setFillColor(hoverColor);
    } else {
        buttonShape.setFillColor(idleColor);
    }
}

bool MapButton::isClicked(const sf::Vector2i mousePosition) const {
    return buttonShape.getGlobalBounds().contains(sf::Vector2f(mousePosition));
}

void MapButton::setText(const std::string& label) {
    buttonText.setString(label);
    // Re-center the text after changing it
    centerText();
}

void MapButton::centerText() {
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                         textRect.position.y + textRect.size.y / 2.0f});
}