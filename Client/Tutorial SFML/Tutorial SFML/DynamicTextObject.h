#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class DynamicTextObject : public Object
{
public:
	sf::Text* text = nullptr;

	DynamicTextObject(sf::Text* _text) : text(_text) {}

	void render(sf::RenderWindow& window) override {
		if (text)
			window.draw(*text);
	}

	void handleEvent(const sf::Event& event) override {}
};