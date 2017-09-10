#ifndef SNAKEIT_BOT_H
#define SNAKEIT_BOT_H

#include "Bot.h"

class SnakeitBot : public Bot
{
	bool fight_mode{ true };
	Board::Route longest_way_to_death;
public:
	void Move(int time);
};

#endif

