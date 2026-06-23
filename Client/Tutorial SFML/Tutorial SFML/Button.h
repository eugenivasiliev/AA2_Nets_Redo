#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "GameStyle.h"
#include <functional>
#include <algorithm>

class Button : public Object
{
public:
	sf::RectangleShape box;
	sf::RectangleShape shadow;
	sf::RectangleShape highlight;
	sf::RectangleShape glow;
	sf::Text label;
	std::function<void()> onClick;

	sf::Color baseColor;
	sf::Color hoverColor;
	sf::Color pressedColor;
	sf::Color outlineColor = sf::Color(255, 255, 255, 36);
	sf::Color glowColor = sf::Color(255, 255, 255, 18);

	bool isHovered = false;
	bool isPressed = false;

	Button() : label(nullFont) {}

	Button(sf::RectangleShape _rect, sf::Text _text, std::function<void()> _onClick)
		: box(_rect), label(_text), onClick(_onClick)
	{
		baseColor = box.getFillColor();

		hoverColor = sf::Color(
			static_cast<std::uint8_t>(std::min(255, static_cast<int>(baseColor.r) + 18)),
			static_cast<std::uint8_t>(std::min(255, static_cast<int>(baseColor.g) + 18)),
			static_cast<std::uint8_t>(std::min(255, static_cast<int>(baseColor.b) + 18)),
			baseColor.a
		);

		pressedColor = sf::Color(
			static_cast<std::uint8_t>(std::max(0, static_cast<int>(baseColor.r) - 24)),
			static_cast<std::uint8_t>(std::max(0, static_cast<int>(baseColor.g) - 24)),
			static_cast<std::uint8_t>(std::max(0, static_cast<int>(baseColor.b) - 24)),
			baseColor.a
		);

		box.setOutlineThickness(2.f);
		box.setOutlineColor(outlineColor);

		shadow.setSize(box.getSize());
		shadow.setPosition({ box.getPosition().x + 4.f, box.getPosition().y + 7.f });
		shadow.setFillColor(sf::Color(0, 0, 0, 85));

		glow.setSize({ box.getSize().x + 8.f, box.getSize().y + 8.f });
		glow.setPosition({ box.getPosition().x - 4.f, box.getPosition().y - 4.f });
		glow.setFillColor(sf::Color(255, 255, 255, 0));
		glow.setOutlineThickness(2.f);
		glow.setOutlineColor(sf::Color(255, 255, 255, 0));

		highlight.setSize({ box.getSize().x, box.getSize().y * 0.32f });
		highlight.setPosition(box.getPosition());
		highlight.setFillColor(sf::Color(255, 255, 255, 20));

		label.setFillColor(sf::Color::White);
		if (label.getCharacterSize() == 30)
			label.setCharacterSize(28);
	}

	bool clickedInside(int posX, int posY) {
		return box.getGlobalBounds().contains({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void render(sf::RenderWindow& window) override {
		shadow.setSize(box.getSize());
		shadow.setPosition({
			box.getPosition().x + (isPressed ? 2.f : 4.f),
			box.getPosition().y + (isPressed ? 3.f : 7.f)
			});

		glow.setSize({ box.getSize().x + 8.f, box.getSize().y + 8.f });
		glow.setPosition({ box.getPosition().x - 4.f, box.getPosition().y - 4.f });

		highlight.setSize({ box.getSize().x, box.getSize().y * 0.32f });
		highlight.setPosition(box.getPosition());

		sf::FloatRect textBounds = label.getLocalBounds();
		label.setPosition({
			box.getPosition().x + (box.getSize().x - textBounds.size.x) / 2.f - textBounds.position.x,
			box.getPosition().y + (box.getSize().y - textBounds.size.y) / 2.f - textBounds.position.y - (isPressed ? 0.f : 2.f)
			});

		if (isHovered) {
			window.draw(glow);
		}
		window.draw(shadow);
		window.draw(box);
		window.draw(highlight);
		window.draw(label);
	}

	void handleEvent(const sf::Event& event) override {
		if (const sf::Event::MouseMoved* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
			isHovered = clickedInside(mouseMoved->position.x, mouseMoved->position.y);

			if (!isPressed) {
				box.setFillColor(isHovered ? hoverColor : baseColor);
				glow.setOutlineColor(isHovered ? glowColor : sf::Color(255, 255, 255, 0));
			}
		}

		if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			if (mousePressed->button == sf::Mouse::Button::Left) {
				if (clickedInside(mousePressed->position.x, mousePressed->position.y)) {
					isPressed = true;
					box.setFillColor(pressedColor);
				}
			}
		}

		if (const sf::Event::MouseButtonReleased* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
			if (mouseReleased->button == sf::Mouse::Button::Left) {
				bool wasInside = clickedInside(mouseReleased->position.x, mouseReleased->position.y);

				if (isPressed && wasInside && onClick) {
					onClick();
				}

				isPressed = false;
				isHovered = wasInside;
				box.setFillColor(isHovered ? hoverColor : baseColor);
				glow.setOutlineColor(isHovered ? glowColor : sf::Color(255, 255, 255, 0));
			}
		}
	}
};