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
		b[x][y] = (line == ".");

		if (line == PLAYER_0)
			playerPos[static_cast<int>(Player::Pl1)] = make_pair(x, y);

		if (line == PLAYER_1)
			playerPos[static_cast<int>(Player::Pl2)] = make_pair(x, y);
		
		x = (x + 1) % 16;

		if (x == 0)
			y++;
	}
}

vector<BoardMoves> Board::LegalMoves(Player pl)
{
	pair<int, int> pos = playerPos[static_cast<int>(pl)];
	vector<BoardMoves> moves;
	if (pos.second > 0 && b[pos.first][pos.second - 1])
		moves.push_back(BoardMoves::UP);
	if (pos.second < 15 && b[pos.first][pos.second + 1]) 
		moves.push_back(BoardMoves::DOWN);
	if (pos.first > 0 && b[pos.first - 1][pos.second]) 
		moves.push_back(BoardMoves::LEFT);
	if (pos.first < 15 && b[pos.first + 1][pos.second]) 
		moves.push_back(BoardMoves::RIGHT);
	return moves;
}
