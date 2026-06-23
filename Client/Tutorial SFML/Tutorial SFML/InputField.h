#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "GameStyle.h"
#include <string>

class InputField : public Object
{
public:
	sf::RectangleShape box;
	sf::Text text;
	std::string value = "";
	std::string displayValue = "";
	int maxChars;

	bool isCensored = false;
	bool isFocused = false;

	sf::Color idleOutlineColor = sf::Color(255, 255, 255, 30);
	sf::Color focusOutlineColor = sf::Color(88, 101, 242, 220);

	InputField() : text(nullFont), maxChars(0) {}

	InputField(sf::RectangleShape _rect, sf::Text _text, int _maxChars)
		: box(_rect), text(_text), maxChars(_maxChars)
	{
		box.setOutlineThickness(2.f);
		box.setOutlineColor(idleOutlineColor);

		if (text.getCharacterSize() == 30)
			text.setCharacterSize(26);

		text.setFillColor(sf::Color(236, 240, 255));
		updateDisplayString();
	}

	bool clickedInside(int posX, int posY) {
		return box.getGlobalBounds().contains({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void setTextCharacterSize(unsigned int size) {
		text.setCharacterSize(size);
	}

	void setTextColor(const sf::Color& color) {
		text.setFillColor(color);
	}

	void setIdleOutlineColor(const sf::Color& color) {
		idleOutlineColor = color;
		if (!isFocused)
			box.setOutlineColor(idleOutlineColor);
	}

	void setFocusOutlineColor(const sf::Color& color) {
		focusOutlineColor = color;
		if (isFocused)
			box.setOutlineColor(focusOutlineColor);
	}

	void setFocused(bool focused) {
		isFocused = focused;
		box.setOutlineColor(isFocused ? focusOutlineColor : idleOutlineColor);
	}

	void setSelected(bool selected) {
		setFocused(selected);
	}

	void updateDisplayString() {
		if (isCensored)
			displayValue = std::string(value.size(), '*');
		else
			displayValue = value;
	}

	void appendCharacter(char c) {
		if (static_cast<int>(value.size()) >= maxChars)
			return;

		value.push_back(c);
		updateDisplayString();
	}

	void removeLastCharacter() {
		if (!value.empty()) {
			value.pop_back();
			updateDisplayString();
		}
	}

	void handleTextEntered(std::uint32_t unicode) {
		if (!isFocused)
			return;

		// Backspace
		if (unicode == 8) {
			removeLastCharacter();
			return;
		}

		if (unicode == 13 || unicode == 9 || unicode == 27) {
			return;
		}

		if (unicode >= 32 && unicode <= 126) {
			appendCharacter(static_cast<char>(unicode));
		}
	}

	void onClick(const sf::Event::MouseButtonPressed* mouse) {
		setFocused(clickedInside(mouse->position.x, mouse->position.y));
	}

	void render(sf::RenderWindow& window) override {
		text.setString(displayValue);

		sf::FloatRect textBounds = text.getLocalBounds();
		text.setPosition({
			box.getPosition().x + 14.f,
			box.getPosition().y + (box.getSize().y - textBounds.size.y) / 2.f - textBounds.position.y - 2.f
			});

		window.draw(box);
		window.draw(text);
	}

	void handleEvent(const sf::Event& event) override {
		if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			if (mousePressed->button == sf::Mouse::Button::Left) {
				onClick(mousePressed);
			}
		}

		if (const sf::Event::TextEntered* textEntered = event.getIf<sf::Event::TextEntered>()) {
			handleTextEntered(textEntered->unicode);
		}
	}
};