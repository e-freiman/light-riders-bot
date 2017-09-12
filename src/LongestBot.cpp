#include "LongestBot.h"

#include <assert.h>

using namespace std;

void LongestBot::Move(int time)
{
	Board::Route route = board.LongestWay(my_id);
	if (!route.empty())
	{
		Position next = *route.begin();
		assert(ToDirection(board.Head(my_id), next) != Directions::NONE);
		MakeMove(ToDirection(board.Head(my_id), next));
	}
	else
	{
		MakeMove(Directions::UP);
	}
}