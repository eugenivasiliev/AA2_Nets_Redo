#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "InputField.h"

class PlaceholderText : public Object
{
public:
    sf::Text text;
    InputField* field;

    PlaceholderText(const sf::Text& _text, InputField* _field) : text(_text), field(_field) {}

    void render(sf::RenderWindow& window) override {
        if (field && field->value.empty()) {
            window.draw(text);
        }
    }

    void handleEvent(const sf::Event& event) override {}
};