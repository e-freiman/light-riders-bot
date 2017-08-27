#include "RandomBot.h"

void RandomBot::Move(int time) {
	auto moves = board.LegalMoves(playerId);
	if (moves.empty())
	{
		MakeMove(BoardMoves::UP);
		return;
	}

	MakeMove(moves[rand() % moves.size()]);
}

