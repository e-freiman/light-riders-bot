#include "BobBot.h"

void BobBot::Move(int time) 
{
	if (fight_mode)
	{
		//Check if we still can fight
		fight_mode = board.AreHeadsConnected(Board::Route());
		if (!fight_mode)
		{
			longest_way_to_death = board.LongestWay(player_id);
			std::reverse(longest_way_to_death.begin(), longest_way_to_death.end());
		}
		else
		{
			//Fighting!!!

		}

	};

	if (!fight_mode)
	{
		//No way to fight, just dying slowly
		Position p = longest_way_to_death.back();
		longest_way_to_death.pop_back();
		MakeMove(ToDirection(board.Head(player_id), p));
	}
}