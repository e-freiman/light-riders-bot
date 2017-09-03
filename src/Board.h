#ifndef BOARD_H
#define BOARD_H

#include <sstream>
#include <vector>
#include <array>
#include "GameEnums.h"
#include <functional>
#include <optional>

constexpr int N_PLAYERS = 2;
constexpr int BOARD_SIZE = 16;

class Board
{
public:
	using Position = std::pair<int, int>;
	using Route = std::vector<Position>;

private:
	std::array<Position, N_PLAYERS> player_positions;
	std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE> board;

public:	
	Board();
	Board(std::stringstream &&stream);

	std::vector<BoardMoves> LegalMoves(Player pl);

	//Traverses game board from the source by using BFS method
	//func - function that is called for each available square (first parameter indicates if square contains a head, the second if square is located near a wall or tail), 
	//if func returns false - traversing stops, otherwise traversing continues
	void BfsTraverse(const Position& source, std::function<bool(std::optional<Player> head, bool near_wall)> func);

	bool AreHeadsConnected(const Route& route);
	int AvailableSpace(Player pl, const Route& route);
	int Distance(Player pl, const Route& route);
};

#endif
