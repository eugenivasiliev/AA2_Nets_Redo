#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class ObjectCircle : public Object
{
public:
	sf::CircleShape shape;

	ObjectCircle(const sf::CircleShape& _shape) : shape(_shape) {}

	void render(sf::RenderWindow& window) override {
		window.draw(shape);
	}

	void handleEvent(const sf::Event& event) override {}
};