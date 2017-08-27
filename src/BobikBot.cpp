#include "BobikBot.h"

void BobikBot::Move(int time) {
	auto moves = board.LegalMoves(playerId);
	if (moves.empty())
	{
		MakeMove(BoardMoves::DOWN);
		return;
	}

	MakeMove(moves[rand() % moves.size()]);
}