#include "SnakeitBot.h"
#include <assert.h>

#include "Logger.h"

using namespace std;

void SnakeitBot::Move(int time)
{	
	static int move_number = 0;

	LOG_INIT("logs.txt");
	LOG(board);
	LOG("Move number = " + std::to_string(move_number++) + ", my head = " + std::to_string(static_cast<int>(my_id)));

	if (move_number == 16)
	{
		bool stop = true;
	}

	std::optional<Position> next;

	//Check if we can fight
	if (board.IsReachableByAnother(my_id, Board::Route()))
	{
		//Fighting!!!
		//Figuring out the beginning of the route that could possible cut the enemy from living space
		LOG("Heads are connected");

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
				auto space = board.AvailableSpace(my_id, route);
				if (space[static_cast<int>(my_id)] > space[static_cast<int>(enemy_id)])
				{					
					int distance_from_enemy = board.Distance(enemy_id, route);
					//Choosing the route based on utility, prefer short startegy far from the enemy
					double current_utilty = (double)distance_from_enemy / (double)route.size();
					LOG("Route to [" + std::to_string(p.first) + "," + std::to_string(p.second) + "], size = " + std::to_string(route.size()) + ", utility = " + std::to_string(current_utilty));
					LOG("Spaces: " + std::to_string(space[static_cast<int>(0)]) + ", " + std::to_string(space[static_cast<int>(1)]));
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
	
	if (!next)
	{
		Board::Route route = board.LongestWay(my_id);
		if (!route.empty())
		{
			LOG("Longest way strategy");
			next = *route.begin();
			assert(ToDirection(board.Head(my_id), next.value()) != Directions::NONE);
		}
	}	
	
	if (next)
	{
		MakeMove(ToDirection(board.Head(my_id), next.value()));
	}
	else
	{
		MakeMove(Directions::UP);
	}	
}