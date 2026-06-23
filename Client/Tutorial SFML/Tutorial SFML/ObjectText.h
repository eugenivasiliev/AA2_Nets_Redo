#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class ObjectText : public Object
{
public:
    sf::Text text;

    ObjectText(const sf::Text& _text) : text(_text) {}

    void render(sf::RenderWindow& window) override {
        window.draw(text);
    }

    void handleEvent(const sf::Event& event) override {}
};