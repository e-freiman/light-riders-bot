#ifndef BOT_H
#define BOT_H

#include "GameEnums.h"
#include "Board.h"

class Bot 
{
protected:
	Players player_id;
	int time_bank;
	int time_per_move;
	std::string bot_name;
	std::string player1, player2;
	
	int round;
	Board board;
	void MakeMove(Directions boardMove);

public:
	Bot();

	// Action
	virtual void Move(int time) = 0;
	
	// Update
	void UpdateRound(int time);
	void UpdateBoard(const Board& board);
	
	// Settings
	void SetTimebank(int time);
	void SetTimePerMove(int time);
	void SetYourBot(const std::string& name);
	void SetYourBotId(Players playerId);
	void SetPlayerNames(const std::string& player1, const std::string& player2);
};

#endif
