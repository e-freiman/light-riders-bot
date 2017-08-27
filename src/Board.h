#ifndef BOARD_H
#define BOARD_H

#include <sstream>
#include <vector>
#include <array>
#include "GameEnums.h"

constexpr int N_PLAYERS = 2;
constexpr int BOARD_SIZE = 16;

struct Board
{
	std::array<std::pair<int, int>, N_PLAYERS> playerPos;
	std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE> b;
	
	Board();
	Board(std::stringstream &&stream);
	std::vector<BoardMoves> LegalMoves(Player pl);
};

#endif
