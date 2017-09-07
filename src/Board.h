#ifndef BOARD_H
#define BOARD_H

#include <sstream>
#include <set>
#include <vector>
#include <queue>
#include <array>
#include "GameEnums.h"
#include <functional>
#include <optional>

constexpr int N_PLAYERS = 2;
constexpr int BOARD_SIZE = 16;

class Board final
{
public:
	using Position = std::pair<int, int>;
	using Route = std::vector<Position>;
	//the first index is to the right (x), the second index is to the down (y)
	template<typename T> using BoardData = std::array<std::array<T, BOARD_SIZE>, BOARD_SIZE>;
	using BoardVisitedData = BoardData<std::optional<std::pair<Directions, int>>>;
	Position MakePosition(int first, int second);

private:
	std::array<Position, N_PLAYERS> pl_pos;
	BoardData<bool> board;
	//s - position from which exploration goes
	//p - position which is explore
	void Explore(Position s, Position p, std::queue<Position>& to_explore, BoardVisitedData& visited, const Route& route);

public:	
	//Creates an empty board
	Board();
	//Creates a board from a stream
	Board(std::stringstream &&stream);	
	//Returns set of available moves
	std::set<Directions> LegalMoves(Players pl);
	//Returns if specified position contains a tail
	bool operator[](const Position& p);
	//Returns position of the head of a specified player
	Position Head(Players pl);
	//Traverses game board from the source by using BFS method, tails board boundaries and route (the second parameter) 
	//are considered as constraints
	//func - function that is called for each available square (the firts parameter indicates position of a square,
	//the second contains an array of squares, value in each square is a pair: first - directs to the square from which we got to 
	//the current square, second - distance from the source)
	//if func returns true - traversing stops (and BfsTraverse returns true), otherwise traversing continues
	//if all available squares were traversed and func was always returning false, BfsTraverse returns false
	//Parameters that are passed to func only garantee to exsit inside func function
	bool BfsTraverse(const Position& source, const Route& route, std::function<bool(const Position&, const BoardVisitedData&)> func);
	//Traverses game board from the source by using DFS method. All parameters are the same as in BfsTraverse method
	bool DfsTraverse(const Position& source, const Route& route, std::function<bool(const Position&, const BoardVisitedData&)> func);
	//Returns true if heads are conected. Specified route is considered as constraints (along with tails and board bondaries)
	bool AreHeadsConnected(const Route& route);
	//Returns available space for for a player. Specified route is considered as constraints (along with tails and board bondaries)
	int AvailableSpace(Players pl, const Route& route);
	//Returns a route from specified player's head to the destination (includes both edges)
	Route RouteTo(Players pl, std::function<bool(const Position& pos)> stopping_criteria);
	//Returns distance from player's head to specified root
	//Uses method RouteTo inside
	//-1 - the route doesnot exist
	//0 - the route goes from the head
	//1 - the route is next to the head
	int Distance(Players pl, const Route& route);
	//Returns the longest possible way for the player (approximated by DFS search)
	Route LongestWay(Players pl);
};

#endif
