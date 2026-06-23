#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Cell.h"
#include "config.h"
#include "GameStyle.h"
#include "Object.h"
#include "PlayerManager.h"

class Board : public Object
{
	PlayerManager* playerManager;
	std::vector<std::vector<Cell>> cells;

public:
	Board(PlayerManager* _playerManager) : playerManager(_playerManager) {
		cells = std::vector<std::vector<Cell>>();
		for (int i = 0; i < BOARD_HEIGHT; ++i) {
			cells.push_back(std::vector<Cell>());
			for (int j = 0; j < BOARD_WIDTH; ++j)
				cells.back().push_back(Cell());
		}
	}

	void update() override {
		Move pending = NT->GetPendingMove();
		if (pending != nullMove) 
			playTurn(pending);
	}

	void render(sf::RenderWindow& window) override {
		const sf::Vector2f boardPosition = {
			WINDOW_WIDTH * boardAnchorX,
			WINDOW_HEIGHT * boardAnchorY
		};

		const float cellWidth = boardSideLength / BOARD_WIDTH;
		const float cellHeight = boardSideLength / BOARD_HEIGHT;

		// Base board
		sf::RectangleShape boardBackground;
		boardBackground.setSize({ boardSideLength, boardSideLength });
		boardBackground.setPosition(boardPosition);
		boardBackground.setFillColor(sf::Color(30, 34, 48));
		boardBackground.setOutlineThickness(3.f);
		boardBackground.setOutlineColor(sf::Color(255, 255, 255, 18));
		window.draw(boardBackground);

		// Top highlight
		sf::RectangleShape boardHighlight;
		boardHighlight.setSize({ boardSideLength, 10.f });
		boardHighlight.setPosition(boardPosition);
		boardHighlight.setFillColor(sf::Color(gameAccentXColor.r, gameAccentXColor.g, gameAccentXColor.b, 120));
		window.draw(boardHighlight);

		// Grid lines
		for (int i = 1; i < BOARD_WIDTH; ++i) {
			sf::RectangleShape vLine;
			vLine.setSize({ 4.f, boardSideLength });
			vLine.setPosition({
				boardPosition.x + cellWidth * i - 2.f,
				boardPosition.y
				});
			vLine.setFillColor(sf::Color(255, 255, 255, 35));
			window.draw(vLine);
		}

		for (int i = 1; i < BOARD_HEIGHT; ++i) {
			sf::RectangleShape hLine;
			hLine.setSize({ boardSideLength, 4.f });
			hLine.setPosition({
				boardPosition.x,
				boardPosition.y + cellHeight * i - 2.f
				});
			hLine.setFillColor(sf::Color(255, 255, 255, 35));
			window.draw(hLine);
		}

		// Hover / active board border when it's the user's turn
		if (playerManager->currentPlayer == playerManager->user.userIndex) {
			sf::RectangleShape activeBorder;
			activeBorder.setSize({ boardSideLength, boardSideLength });
			activeBorder.setPosition(boardPosition);
			activeBorder.setFillColor(sf::Color::Transparent);
			activeBorder.setOutlineThickness(3.f);
			activeBorder.setOutlineColor(sf::Color(gameAccentOColor.r, gameAccentOColor.g, gameAccentOColor.b, 140));
			window.draw(activeBorder);
		}

		// Cells and marks
		for (int row = 0; row < BOARD_HEIGHT; ++row) {
			for (int column = 0; column < BOARD_WIDTH; ++column) {
				const sf::Vector2f cellPosition = {
					boardPosition.x + cellWidth * column,
					boardPosition.y + cellHeight * row
				};

				const sf::Vector2f innerSize = {
					cellWidth * (1.f - cellPadding),
					cellHeight * (1.f - cellPadding)
				};

				const sf::Vector2f innerPosition = {
					cellPosition.x + (cellWidth - innerSize.x) / 2.f,
					cellPosition.y + (cellHeight - innerSize.y) / 2.f
				};

				// Subtle cell background
				sf::RectangleShape cellBackground;
				cellBackground.setSize(innerSize);
				cellBackground.setPosition(innerPosition);
				cellBackground.setFillColor(sf::Color(38, 42, 58));
				window.draw(cellBackground);

				const int playerIndex = cells[row][column].playerIndex;
				if (playerIndex < 0)
					continue;

				// Draw X / O style marks instead of flat colors
				if (playerIndex % 2 == 0) {
					// X
					sf::RectangleShape line1;
					line1.setSize({ innerSize.x * 0.65f, 6.f });
					line1.setOrigin({ line1.getSize().x / 2.f, line1.getSize().y / 2.f });
					line1.setPosition({
						innerPosition.x + innerSize.x / 2.f,
						innerPosition.y + innerSize.y / 2.f
						});
					line1.setFillColor(colours[playerIndex + 1]);
					line1.setRotation(sf::degrees(45.f));
					window.draw(line1);

					sf::RectangleShape line2 = line1;
					line2.setRotation(sf::degrees(-45.f));
					window.draw(line2);
				}
				else {
					// O
					sf::CircleShape circle;
					float radius = std::min(innerSize.x, innerSize.y) * 0.26f;
					circle.setRadius(radius);
					circle.setOrigin({ radius, radius });
					circle.setPosition({
						innerPosition.x + innerSize.x / 2.f,
						innerPosition.y + innerSize.y / 2.f
						});
					circle.setFillColor(sf::Color::Transparent);
					circle.setOutlineThickness(6.f);
					circle.setOutlineColor(colours[playerIndex + 1]);
					window.draw(circle);
				}
			}
		}
	}

	void handleEvent(const sf::Event& event) override {
		if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			switch (mousePressed->button)
			{
			case sf::Mouse::Button::Left:
				std::cout << "Click" << std::endl;
				if (!validClickPos(mousePressed->position.x, mousePressed->position.y) || playerManager->currentPlayer != NT->GetPlayerIndex()) 
					return;
				std::cout << "Valid move" << std::endl;
				playTurn(Move(screenToBoardY(mousePressed->position.y), screenToBoardX(mousePressed->position.x), playerManager->currentPlayer));
				break;
			default:
				break;
			}
		}
	}

private:
	inline bool validClickPos(int posX, int posY) {
		return
			WINDOW_WIDTH * boardAnchorX <= posX &&
			WINDOW_WIDTH * boardAnchorX + boardSideLength >= posX &&
			WINDOW_HEIGHT * boardAnchorY <= posY &&
			WINDOW_HEIGHT * boardAnchorY + boardSideLength >= posY;
	}

	inline int screenToBoardX(int x) {
		return (int)(((float)x - WINDOW_WIDTH * boardAnchorX) / boardSideLength * BOARD_WIDTH);
	}

	inline int screenToBoardY(int y) {
		return (int)(((float)y - WINDOW_HEIGHT * boardAnchorY) / boardSideLength * BOARD_HEIGHT);
	}

	bool setCell(int row, int column, int player) {
		if (cells[row][column].playerIndex >= 0)
			return false;

		cells[row][column].playerIndex = player;
		return true;
	}

	bool checkWin(int row, int column) {
		for (int i = -2; i <= 0; ++i)
			if (
				checkLine(row + i, column, row + i + 1, column, row + i + 2, column) ||
				checkLine(row, column + i, row, column + i + 1, row, column + i + 2) ||
				checkLine(row + i, column + i, row + i + 1, column + i + 1, row + i + 2, column + i + 2) ||
				checkLine(row - i, column + i, row - i - 1, column + i + 1, row - i - 2, column + i + 2)
				)
				return true;

		return false;
	}

	inline bool checkLine(int row1, int column1, int row2, int column2, int row3, int column3) {
		if (!validCell(row1, column1) || !validCell(row2, column2) || !validCell(row3, column3))
			return false;

		return cells[row1][column1].playerIndex >= 0 &&
			cells[row1][column1].playerIndex == cells[row2][column2].playerIndex &&
			cells[row2][column2].playerIndex == cells[row3][column3].playerIndex;
	}

	inline bool validCell(int row, int column) {
		return row >= 0 && row < BOARD_HEIGHT && column >= 0 && column < BOARD_WIDTH;
	}

	void playTurn(Move move) {
		std::cout << "Play" << std::endl;

		if (move == noMove) {
			playerManager->GetNextPlayer();
			return;
		}

		if (!setCell(move.row, move.column, move.playerIndex))
			return;

		if (checkWin(move.row, move.column))
			playerManager->winners[move.playerIndex] = true;

		if(move.playerIndex == NT->GetPlayerIndex()) NT->SendMove(move);

		playerManager->GetNextPlayer();
	}
};