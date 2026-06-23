#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class ObjectShape : public Object
{
public:
    sf::RectangleShape shape;

    ObjectShape(const sf::RectangleShape& _shape) : shape(_shape) {}

    void render(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void handleEvent(const sf::Event& event) override {}
};