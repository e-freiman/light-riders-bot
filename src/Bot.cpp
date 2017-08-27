#include "Bot.h"
#include <iostream>
using namespace std;

const char* BOARD_MOVES[4] = { "up", "down", "left", "right" };

Bot::Bot() 
	: board() 
{
}

void Bot::MakeMove(BoardMoves boardMove) 
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
	this->timeBank = time;
};

void Bot::SetTimePerMove(int time) 
{
	this->timePerMove = time;
};

void Bot::SetYourBot(const string& name) 
{
	this->botName = name;
};

void Bot::SetYourBotId(Player playerId) 
{ 
	this->playerId = playerId;  
};

void Bot::SetPlayerNames(const string& player1, const string& player2) 
{
	this->player1 = player1;
	this->player2 = player2;
};