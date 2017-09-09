#ifndef BOB_BOT_H
#define BOB_BOT_H

#include "Bot.h"

class BobBot : public Bot
{
	bool fight_mode{ true };
	Board::Route longest_way_to_death;
public:
	void Move(int time);
};

#endif

