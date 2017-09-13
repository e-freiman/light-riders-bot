#include "SnakeitBot.h"
#include <assert.h>

#define LOGS_ENABLED
#include "Logger.h"

using namespace std;

void SnakeitBot::Move(int time)
{	
	LOG_INIT("logs.txt");
	LOG(board);
	LOG("Head: " + std::to_string(static_cast<int>(my_id)));

	std::optional<Position> next;

	//Check if we can fight
	if (board.IsReachableByAnother(my_id, Board::Route()))
	{
		//Fighting!!!
		//Figuring out the beginning of the route that could possible cut the enemy from living space
		LOG("Head: " + std::to_string(static_cast<int>(my_id)) + " IsReachableByAnother = true");
		

		//Measure by which some routes are more prefered than another
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
					//Choosing the route based on utility, prefer short startegy far from the enemy
					double current_utilty = (double)distance_from_enemy / (double)route.size();
					if (current_utilty > utility)
					{
						utility = current_utilty;
						next = *route.begin();
						assert(ToDirection(board.Head(my_id), next.value()) != Directions::NONE);
					}
				}
			}

			return false;
		};

		board.BfsTraverse(board.Head(my_id), Board::Route(), false, predicate);			
	}
	
	if (!next.has_value())
	{
		Board::Route route = board.LongestWay(my_id);
		if (!route.empty())
		{
			next = *route.begin();
			assert(ToDirection(board.Head(my_id), next.value()) != Directions::NONE);
		}
	}	
	
	if (next.has_value())
	{
		MakeMove(ToDirection(board.Head(my_id), next.value()));
	}
	else
	{
		MakeMove(Directions::UP);
	}	
}