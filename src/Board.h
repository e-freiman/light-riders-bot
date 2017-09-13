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

#include "Position.h"

constexpr int N_PLAYERS = 2;
constexpr int BOARD_SIZE = 16;

class Board final
{
	friend class Logger;
public:
	using Route = std::vector<Position>;
	//the first index is to the right (x), the second index is to the down (y)
	template<typename T> using BoardData = std::array<std::array<T, BOARD_SIZE>, BOARD_SIZE>;
		
	struct BoardVisitedData final : BoardData<std::optional<std::pair<Directions, int>>>
	{
		//Returns a route from sepcified position to a source (direction of the route depends on the second parameter)
		Route RouteToSource(Position p, bool reversed = false) const;
	};

	using TraverseCallback = std::function<bool(const Position&, const BoardVisitedData&)>;

private:
	std::array<Position, N_PLAYERS> pl_pos;
	BoardData<bool> board;
	//Considers boundaries, tails only, visited and route
	bool IsPositionLegal(const Position& p, const BoardVisitedData& visited, const Route& route, bool traversable_heads) const;
	//Consideres boundaries and tails only
	bool IsPositionLegal(const Position& p, bool traversable_heads) const;
	//s - position from which exploration goes
	//p - position which is explored
	void BfsExplore(const Position& s, const Position& p, std::queue<Position>& to_explore, 
		BoardVisitedData& visited, const Route& route, bool traversable_heads) const;
	bool DfsExplore(const Position& s, const Position& p, const Route& route, TraverseCallback func, 
		BoardVisitedData& visited, bool traversable_heads) const;
public:	
	//Creates an empty board
	Board();
	//Creates a board from a stream
	Board(std::stringstream &&stream);	
	//Returns set of available moves
	std::set<Directions> LegalMoves(Players pl) const;
	//Returns if specified position contains a tail
	bool operator[](const Position& p) const;
	//Returns position of the head of a specified player
	Position Head(Players pl) const;
	//Traverses game board from the source by using BFS method, tails board boundaries and route (the second parameter) 
	//are considered as constraints
	//func - function that is called for each available square (the firts parameter indicates position of a square,
	//the second contains an array of squares, value in each square is a pair: first - directs to the square from which we got to 
	//the current square, second - distance from the source)
	//if func returns true - traversing stops (and BfsTraverse returns true), otherwise traversing continues
	//if all available squares were traversed and func was always returning false, BfsTraverse returns false
	//Parameters that are passed to func only garantee to exsit inside func function
	bool BfsTraverse(const Position& source, const Route& route, bool traversable_heads, TraverseCallback func) const;
	//Traverses game board from the source by using DFS method. All parameters are the same as in BfsTraverse method
	bool DfsTraverse(const Position& source, const Route& route, bool traversable_heads, TraverseCallback func) const;
	//Returns true if specified player's head is reachable from another player's head. 
	//Specified route is considered as constraints (along with tails and board bondaries)
	//If player1's head leans on the route then is isn't reachable from player2 but player2's head is reachable from player1
	bool IsReachableByAnother(Players pl, const Route& route) const;
	//Returns available space for for a player. Specified route is considered as constraints (along with tails and board bondaries)
	//It's supposed that the route belongs to specified player.
	std::array<int, N_PLAYERS> AvailableSpace(Players pl, const Route& route) const;
	//Returns a route from specified player's head to the destination (includes both edges)
	Route RouteTo(Players pl, std::function<bool(const Position& pos)> stopping_criteria) const;
	//Returns distance from player's head to specified root
	//Uses method RouteTo inside
	//-1 - the route doesnot exist
	//0 - the route goes from the head
	//1 - the route is next to the head
	int Distance(Players pl, const Route& route) const;
	//Returns the longest possible way for the player (approximated by DFS search)
	Route LongestWay(Players pl) const;
	//Returns true if specified position has any obstacle around it (boundary, tail or head)
	bool IsAnyObstacleAround(const Position& pos) const;
};

#endif