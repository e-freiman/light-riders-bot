#include "RandomBot.h"

void RandomBot::Move(int time) {
	auto moves = board.LegalMoves(my_id);
	if (moves.empty())
	{
		MakeMove(Directions::UP);
		return;
	}


	decltype(moves)::const_iterator selected;
	
	int i = 0;
	int selected_i = rand() % moves.size();

	for (selected = moves.cbegin(); selected != moves.cend(); selected++)
	{
		if (i++ == selected_i) break;
	}

	MakeMove(*selected);
}

