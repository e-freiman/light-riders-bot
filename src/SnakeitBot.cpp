#include "SnakeitBot.h"

using namespace std;

void SnakeitBot::Move(int time)
{
	Position next;

	if (fight_mode)
	{
		//Check if we still can fight (should make decision at each step)
		fight_mode = board.IsReachableByAnother(my_id, Board::Route());
		if (!fight_mode)
		{
			longest_way_to_death = board.LongestWay(my_id);
			std::reverse(longest_way_to_death.begin(), longest_way_to_death.end());
		}
		else
		{
			//Fighting!!!

			//Figuring out the beginning of the route that could possible cut the enemy from living space

			//Measure by which one "cutting" route is more prefered than another
			double utility{ 0.0 };

			auto predicate = [board = board, my_id = my_id, enemy_id = enemy_id, &next, &utility](const auto& p, const auto& visited)
			{
				//An obstacle nearby means that it worth to check a position for "cutting" property
				if (board.IsAnyObstacleAround(p))
				{
					Board::Route route = visited.RouteToSource(p, true);
					//After cutting we should have more space, otherwise it is suicide,
					//if the route doesn't cut anybody then available space is going to be the same
					auto space = board.AvailableSpace(route);
					if (space[static_cast<int>(my_id)] > space[static_cast<int>(enemy_id)])
					{
						int distance_from_enemy = board.Distance(enemy_id, route);
						if (distance_from_enemy > route.size())
						{
							//The win is garanteed, since the enemy doesn't have time to intercept our startegy
							next = *route.begin();
							return true;
						}
						else
						{
							//Choosing the route based on utility, prefer short startegy far from the enemy
							double current_utilty = (double)distance_from_enemy / (double)route.size();
							if (current_utilty > utility)
							{
								utility = current_utilty;
								next = *route.begin();
							}
						}
					}
				}

				return false;
			};

			board.BfsTraverse(board.Head(my_id), Board::Route(), false, predicate);			
		}

	};

	if (!fight_mode)
	{
		//No way to fight, just dying slowly
		next = longest_way_to_death.back();
		longest_way_to_death.pop_back();
	}

	MakeMove(ToDirection(board.Head(my_id), next));
}