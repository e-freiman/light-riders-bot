#include "Bot.h"
#include <iostream>
using namespace std;

const char* BOARD_MOVES[4] = { "up", "down", "left", "right" };

Bot::Bot() 
	: board() 
{
}

void Bot::MakeMove(Directions boardMove) 
{
	cout << BOARD_MOVES[static_cast<int>(boardMove)] << endl;
}

void Bot::UpdateRound(int round) 
{
	this->round = round;
};

void Bot::UpdateBoard(const Board& board) 
{ 
	this->board = board; 
};

void Bot::SetTimebank(int time) 
{
	this->time_bank = time;
};

void Bot::SetTimePerMove(int time) 
{
	this->time_per_move = time;
};

void Bot::SetYourBot(const string& name) 
{
	this->bot_name = name;
};

void Bot::SetYourBotId(Players playerId) 
{ 
	this->my_id = playerId;  
	this->enemy_id = my_id == Players::PL1 ? Players::PL2 : Players::PL1;
};

void Bot::SetPlayerNames(const string& player1, const string& player2) 
{
	this->player1 = player1;
	this->player2 = player2;
};