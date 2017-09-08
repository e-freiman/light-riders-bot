#include "Board.h"

using namespace std;

constexpr auto PLAYER_0 = "0";
constexpr auto PLAYER_1 = "1";

Board::Board() {}

Board::Board(stringstream &&stream)
{
	int x = 0, y = 0;
	string line;
	while (getline(stream, line, ','))
	{
		board[x][y] = (line == ".");

		if (line == PLAYER_0)
			pl_pos[static_cast<int>(Players::Pl1)] = Position(x, y);

		if (line == PLAYER_1)
			pl_pos[static_cast<int>(Players::Pl2)] = Position(x, y);

		x = (x + 1) % BOARD_SIZE;

		if (x == 0)
			y++;
	}
}

set<Directions> Board::LegalMoves(Players pl)
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

bool Board::IsPositionLegal(const Position& p, const BoardVisitedData& visited, const Route& route)
{
	bool in_boundaries = p.first > 0 && p.first < BOARD_SIZE && p.second > 0 && p.second < BOARD_SIZE;
	bool hit_tail = board[p.first][p.second] && (std::find(pl_pos.cbegin(), pl_pos.cend(), p) == pl_pos.cend());
	bool hit_route = std::find(route.cbegin(), route.cend(), p) != route.cend();
	return in_boundaries && !visited[p.first][p.second].has_value() && !hit_tail && !hit_route;
}

Directions Board::ToDirection(const Position& begin, const Position& end)
{
	if (begin.first < end.first) return Directions::RIGHT;
	if (begin.first > end.first) return Directions::LEFT;
	if (begin.second < end.second) return Directions::DOWN;
	if (begin.second > end.second) return Directions::UP;
	
	return Directions::NONE;
}

void Board::BfsExplore(const Position& s, const Position& p, std::queue<Position>& to_explore, BoardVisitedData& visited, const Route& route)
{
	if (IsPositionLegal(p, visited, route))
	{
		int distance = visited[s.first][s.second].value().second + 1;
		visited[p.first][p.second] = std::make_pair(ToDirection(p, s), distance);
		to_explore.push(p);
	}
}

Position Board::MakePosition(int first, int second)
{
	Position p;
	p.first = first;
	p.second = second;
	return p;
}

bool Board::BfsTraverse(const Position& source, const Route& route, TraverseCallback func)
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
			BfsExplore(square, neigbor, to_explore, visited, route);
		}
	}

	return false;
}

bool Board::DfsExplore(const Position& s, const Position& p, const Route& route, TraverseCallback func, BoardVisitedData& visited)
{
	int distance = s == p ? 0 : visited[s.first][s.second].value().second + 1;
	visited[p.first][p.second] = std::make_pair(ToDirection(p, s), distance);

	if (func(p, visited))
	{
		return true;
	}

	for (auto neigbor : p)
	{		
		if (IsPositionLegal(neigbor, visited, route))
			if (DfsExplore(p, neigbor, route, func, visited))
				return true;
	}

	return false;
}

bool Board::DfsTraverse(const Position& source, const Route& route, TraverseCallback func)
{
	BoardVisitedData visited;
	return DfsExplore(source, source, route, func, visited);
}

bool Board::AreHeadsConnected(const Route& route)
{
	auto predicate = [p2_pos = pl_pos[static_cast<int>(Players::Pl2)]](const auto& p, const auto&)
	{
		return p == p2_pos;
	};

	return BfsTraverse(pl_pos[static_cast<int>(Players::Pl1)], route, predicate);
}

int Board::AvailableSpace(Players pl, const Route& route)
{
	int space = 0;
	BfsTraverse(pl_pos[static_cast<int>(pl)], route, [&space](const auto& p, const auto&) {space++; return false; });
	return space;
}

Board::Route Board::RouteTo(Players pl, function<bool(const Position& pos)> stopping_criteria)
{
	Route route;

	auto predicate = [&route, &stopping_criteria](const auto& p, const auto& visited)
	{
		if (stopping_criteria(p))
		{
			Position pos = p;

			while (visited[pos.first][pos.second].value().first != Directions::NONE)
			{
				route.emplace(route.begin(), pos);

				switch (visited[pos.first][pos.second].value().first)
				{
				case Directions::UP:
					pos.second--;
					break;
				case Directions::DOWN:
					pos.second++;
					break;
				case Directions::LEFT:
					pos.first--;
					break;
				case Directions::RIGHT:
					pos.first++;
					break;
				default:
					throw invalid_argument("Unknown direction");
				}
			}

			route.emplace(route.begin(), pos);
			return true;
		}
		return false;
	};

	BfsTraverse(pl_pos[static_cast<int>(pl)], Route(), predicate);
	return route;
}

int Board::Distance(Players pl, const Route& route)
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

	BfsTraverse(pl_pos[static_cast<int>(pl)], Route(), predicate);

	return distance;
}

bool Board::operator[](const Position& p)
{
	return board[p.first][p.second];
}

Position Board::Head(Players pl)
{
	return pl_pos[static_cast<int>(pl)];
}

Board::Route Board::LongestWay(Players pl)
{
	Route route;

	//TODO

	return route;
}