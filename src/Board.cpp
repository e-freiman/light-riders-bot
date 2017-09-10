#include <assert.h>

#include "Board.h"

using namespace std;

constexpr auto PLAYER_0 = "0";
constexpr auto PLAYER_1 = "1";

Board::Route Board::BoardVisitedData::RouteToSource(Position p, bool reversed) const
{
	Route route;
	while ((*this)[p.first][p.second].value().first != Directions::NONE)
	{
		reversed ? route.emplace(route.begin(), p) : route.emplace(route.end(), p);

		switch ((*this)[p.first][p.second].value().first)
		{
		case Directions::UP:
			p.second--;
			break;
		case Directions::DOWN:
			p.second++;
			break;
		case Directions::LEFT:
			p.first--;
			break;
		case Directions::RIGHT:
			p.first++;
			break;
		default:
			throw invalid_argument("Unknown direction");
		}
	}

	reversed ? route.emplace(route.begin(), p) : route.emplace(route.end(), p);

	return route;
}

Board::Board() {}

Board::Board(stringstream &&stream)
{
	int x = 0, y = 0;
	string line;
	while (getline(stream, line, ','))
	{
		board[x][y] = (line == ".");

		if (line == PLAYER_0)
			pl_pos[static_cast<int>(Players::PL1)] = Position(x, y);

		if (line == PLAYER_1)
			pl_pos[static_cast<int>(Players::PL2)] = Position(x, y);

		x = (x + 1) % BOARD_SIZE;

		if (x == 0)
			y++;
	}
}

set<Directions> Board::LegalMoves(Players pl) const
{
	Position pos = pl_pos[static_cast<int>(pl)];
	set<Directions> moves;
	if (pos.second > 0 && board[pos.first][pos.second - 1])
		moves.insert(Directions::UP);
	if (pos.second < BOARD_SIZE - 1 && board[pos.first][pos.second + 1])
		moves.insert(Directions::DOWN);
	if (pos.first > 0 && board[pos.first - 1][pos.second])
		moves.insert(Directions::LEFT);
	if (pos.first < BOARD_SIZE - 1 && board[pos.first + 1][pos.second])
		moves.insert(Directions::RIGHT);
	return moves;
}

bool Board::IsPositionLegal(const Position& p, bool traversable_heads) const
{
	bool in_boundaries = p.first > 0 && p.first < BOARD_SIZE && p.second > 0 && p.second < BOARD_SIZE;
	bool hit_head = std::find(pl_pos.cbegin(), pl_pos.cend(), p) != pl_pos.cend();
	bool hit_tail = board[p.first][p.second] && !hit_head;
	
	return in_boundaries && !hit_tail && (traversable_heads || !hit_head);
}

bool Board::IsPositionLegal(const Position& p, const BoardVisitedData& visited, 
	const Route& route, bool traversable_heads) const
{
	bool legal_on_board = IsPositionLegal(p, traversable_heads);
	bool hit_route = std::find(route.cbegin(), route.cend(), p) != route.cend();
	return legal_on_board && !visited[p.first][p.second].has_value() && !hit_route;
}

void Board::BfsExplore(const Position& s, const Position& p, std::queue<Position>& to_explore, 
	BoardVisitedData& visited, const Route& route, bool traversable_heads) const
{
	if (IsPositionLegal(p, visited, route, traversable_heads))
	{
		int distance = visited[s.first][s.second].value().second + 1;
		visited[p.first][p.second] = std::make_pair(ToDirection(p, s), distance);
		to_explore.push(p);
	}
}

bool Board::BfsTraverse(const Position& source, const Route& route, bool traversable_heads, TraverseCallback func) const
{
	BoardVisitedData visited;

	queue<Position> to_explore;
	to_explore.push(source);
	visited[source.first][source.second] = std::make_pair(Directions::NONE, 0);

	while (!to_explore.empty())
	{
		const auto square = to_explore.front();
		to_explore.pop();

		if (func(square, visited))
		{
			return true;
		}

		for (auto neigbor : square)
		{
			BfsExplore(square, neigbor, to_explore, visited, route, traversable_heads);
		}
	}

	return false;
}

bool Board::DfsExplore(const Position& s, const Position& p, const Route& route, TraverseCallback func, 
	BoardVisitedData& visited, bool traversable_heads) const
{
	int distance = s == p ? 0 : visited[s.first][s.second].value().second + 1;
	visited[p.first][p.second] = std::make_pair(ToDirection(p, s), distance);

	if (func(p, visited))
	{
		return true;
	}

	for (auto neigbor : p)
	{		
		if (IsPositionLegal(neigbor, visited, route, traversable_heads))
			if (DfsExplore(p, neigbor, route, func, visited, traversable_heads))
				return true;
	}

	return false;
}

bool Board::DfsTraverse(const Position& source, const Route& route, bool traversable_heads, TraverseCallback func) const
{
	BoardVisitedData visited;
	return DfsExplore(source, source, route, func, visited, traversable_heads);
}

bool Board::IsReachableByAnother(Players pl, const Route& route) const
{
	auto this_head = pl_pos[static_cast<int>(pl)];
	auto another_head = pl_pos[static_cast<int>(pl == Players::PL1 ? Players::PL2 : Players::PL1)];

	auto predicate = [&this_head](const auto& p, const auto&)
	{
		return p == this_head;
	};

	return BfsTraverse(another_head, route, true, predicate);
}

std::array<int, N_PLAYERS> Board::AvailableSpace(const Route& route) const
{
	Players pl_outside_route =
		std::find(route.cbegin(), route.cend(), pl_pos[static_cast<int>(Players::PL1)]) == route.cend() ?
		Players::PL1 : Players::PL2;

	Players pl_2 = pl_outside_route == Players::PL1 ? Players::PL2 : Players::PL1;

	assert(std::find(route.cbegin(), route.cend(), pl_pos[static_cast<int>(pl_outside_route)]) == route.cend());

	std::array<int, N_PLAYERS> res;

	if (std::find(route.cbegin(), route.cend(), pl_pos[static_cast<int>(pl_2)]) == route.cend())
	{
		//Both player's heads are outside the route

		int space = 0;
		BfsTraverse(pl_pos[static_cast<int>(pl_outside_route)], route, false, [&space](const auto& p, const auto&) {space++; return false; });
		res[static_cast<int>(pl_outside_route)] = space;

		space = 0;
		BfsTraverse(pl_pos[static_cast<int>(pl_2)], route, false, [&space](const auto& p, const auto&) {space++; return false; });
		res[static_cast<int>(pl_2)] = space;
	}
	else
	{
		//Second player's head is on the route

		int space = 0;
		BoardVisitedData local_visited;

		BfsTraverse(pl_pos[static_cast<int>(pl_outside_route)], route, false, [&space, &local_visited](const auto& p, const auto& visited)
		{
			space++; 
			local_visited[p.first][p.second] = visited[p.first][p.second]; 
			return false; 
		});

		res[static_cast<int>(pl_outside_route)] = space;

		//Figuring out source for second player traversing
		space = 0;

		for (auto p : route.back())
		{
			if (!local_visited[p.first][p.second].has_value() && std::find(route.cbegin(), route.cend(), p) == route.cend())
			{
				BfsTraverse(p, route, false, [&space](const auto& p, const auto&) {space++; return false; });
				break;
			}
		}
		
		res[static_cast<int>(pl_2)] = space;
	}
	
	
	return res;
}

Board::Route Board::RouteTo(Players pl, function<bool(const Position& pos)> stopping_criteria) const
{
	Route route;

	auto predicate = [&route, &stopping_criteria](const auto& p, const auto& visited)
	{
		if (stopping_criteria(p))
		{
			route = visited.RouteToSource(p, true);
			return true;
		}
		return false;
	};

	BfsTraverse(pl_pos[static_cast<int>(pl)], Route(), false, predicate);
	return route;
}

int Board::Distance(Players pl, const Route& route) const
{
	int distance = -1;
	auto predicate = [&route, &distance](const auto& p, const auto& visited)
	{
		if (std::find(route.cbegin(), route.cend(), p) != route.cend())
		{
			distance = visited[p.first][p.second].value().second;
			return true;
		}
		return false;
	};

	BfsTraverse(pl_pos[static_cast<int>(pl)], Route(), true, predicate);

	return distance;
}

bool Board::operator[](const Position& p) const
{
	return board[p.first][p.second];
}

Position Board::Head(Players pl) const
{
	return pl_pos[static_cast<int>(pl)];
}

Board::Route Board::LongestWay(Players pl) const
{
	BoardVisitedData local_visited;
	Position farest_pos = pl_pos[static_cast<int>(pl)];

	auto predicate = [&local_visited, &farest_pos](const auto& p, const auto& visited)
	{
		local_visited[p.first][p.second] = visited[p.first][p.second];
		if (visited[p.first][p.second].value().second > visited[farest_pos.first][farest_pos.second].value().second)
		{
			farest_pos = p;
		}

		return false;
	};

	DfsTraverse(pl_pos[static_cast<int>(pl)], Route(), false, predicate);
	return local_visited.RouteToSource(farest_pos, true);
}

bool Board::IsAnyObstacleAround(const Position& pos) const
{
	//Traversing up, left, down, right
	for (auto p : pos)
	{
		if (!IsPositionLegal(p, false))
			return true;
	}

	//Traversing diaginal neighbors
	return !(IsPositionLegal(Position(pos.first + 1, pos.second + 1), false) &&
		IsPositionLegal(Position(pos.first + 1, pos.second - 1), false) &&
		IsPositionLegal(Position(pos.first - 1, pos.second + 1), false) &&
		IsPositionLegal(Position(pos.first - 1, pos.second - 1), false));
}