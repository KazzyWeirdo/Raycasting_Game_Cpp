#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class MapButton {
public:
    MapButton(float x, float y, float width, float height, 
              const std::string& text, const sf::Font& font);

    void draw(sf::RenderWindow& window) const;
    void update(sf::Vector2i mousePosition);
    bool isClicked(const sf::Vector2i mousePosition) const;  
    void setText(const std::string& label);
    const std::string getText() const {return this->mapName;};
    
private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Color idleColor;
    sf::Color hoverColor;
    std::string mapName;

    void centerText();
};