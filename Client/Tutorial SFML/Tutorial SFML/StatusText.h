#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include <string>

class StatusText : public Object
{
public:
    sf::Text text;
    std::string* messageRef = nullptr;
    sf::Color* colorRef = nullptr;

    StatusText(const sf::Text& _text, std::string* _messageRef, sf::Color* _colorRef)
        : text(_text), messageRef(_messageRef), colorRef(_colorRef) {
    }

    void render(sf::RenderWindow& window) override {
        if (messageRef) {
            text.setString(*messageRef);
        }

        if (colorRef) {
            text.setFillColor(*colorRef);
        }

        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition({
            window.getSize().x / 2.f - bounds.size.x / 2.f - bounds.position.x,
            text.getPosition().y
            });

        window.draw(text);
    }

    void handleEvent(const sf::Event& event) override {}
};