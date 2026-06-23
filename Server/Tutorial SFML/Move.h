#pragma once
struct Move
{
	int row;
	int column;
	int playerIndex;

	Move(int _row, int _column, int _playerIndex) : row(_row), column(_column), playerIndex(_playerIndex) {}

	bool operator== (Move other) { return row == other.row && column == other.column && playerIndex == other.playerIndex; }
	bool operator!= (Move other) { return !(*this == other); }
};

const Move noMove = { -1, 0, -1 };
const Move nullMove = { -1, -1, -1 };

