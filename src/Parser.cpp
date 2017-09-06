#include "Parser.h"

#include <iostream>

using namespace std;

constexpr auto CMD_ACTION = "action";
constexpr auto CMD_UPDATE = "update";
constexpr auto CMD_SETTINGS = "settings";

constexpr auto ACTION_MOVE = "move";

constexpr auto UPDATE_ROUND = "round";
constexpr auto UPDATE_FIELD = "field";

constexpr auto SETTING_TIMEBANK = "timebank";
constexpr auto SETTING_TIME_PER_MOVE = "time_per_move";
constexpr auto SETTING_YOUR_BOT = "your_bot";
constexpr auto SETTING_YOUR_BOTID = "your_botid";
constexpr auto SETTING_FIELD_WIDTH = "field_width";
constexpr auto SETTING_FIELD_HEIGHT = "field_height";
constexpr auto SETTING_PLAYER_NAMES = "player_names";


constexpr unsigned long long s2i(const char* str, unsigned long long h = 0)
{
	return !str[h] ? 5381 : (s2i(str, h + 1) * 33) ^ str[h];
}

Parser::Parser(shared_ptr<Bot> bot)
	: bot{ bot }
{
}

string Parser::NextCmd() 
{
	string k;
	getline(cmdLine, k, ' ');
	return k;
}

void Parser::Parse() 
{
	string line;
	while (getline(cin, line))
	{
		cmdLine.clear();
		cmdLine.str(line);
	
		// Process command
		string cmd = NextCmd();

		switch (s2i(cmd.c_str()))
		{
		case s2i(CMD_ACTION):
			ProcessAction();
			break;
		case s2i(CMD_UPDATE):
			ProcessUpdate();
			break;
		case s2i(CMD_SETTINGS):
			ProcessSettings();
			break;
		default:
			throw invalid_argument("Unknown command");
		}
	}
}

void Parser::ProcessAction() 
{
	string cmd = NextCmd();
	if (cmd == ACTION_MOVE) 
		bot->Move(stoi(NextCmd()));
	else
		throw invalid_argument("Unknown action");
}

void Parser::ProcessUpdate() {
	NextCmd();
	string cmd = NextCmd();

	switch (s2i(cmd.c_str()))
	{
	case s2i(UPDATE_ROUND):
		bot->UpdateRound(stoi(NextCmd()));
		break;
	case s2i(UPDATE_FIELD):
		{	
			stringstream ss(NextCmd());
			Board board(std::move(ss));
			bot->UpdateBoard(board);
		}
		break;
	default:
		throw invalid_argument("Unknown update");
	}
}

void Parser::ProcessSettings() {
	string cmd = NextCmd();

	switch (s2i(cmd.c_str()))
	{
	case s2i(SETTING_TIMEBANK):
		bot->SetTimebank(stoi(NextCmd()));
		break;
	case s2i(SETTING_TIME_PER_MOVE):
		bot->SetTimePerMove(stoi(NextCmd()));
		break;
	case s2i(SETTING_YOUR_BOT):
		bot->SetYourBot(NextCmd());
		break;
	case s2i(SETTING_YOUR_BOTID):
	{
		int id = stoi(NextCmd());
		bot->SetYourBotId((id == 0) ? Players::Pl1 : Players::Pl2);
		break;
	}
	case s2i(SETTING_FIELD_WIDTH):
	{
		int width = stoi(NextCmd());
		if (width != BOARD_SIZE)
			throw out_of_range("Wrong width of the board");
		break;
	}
	case s2i(SETTING_FIELD_HEIGHT):
	{
		int height = stoi(NextCmd());
		if (height != BOARD_SIZE)
			throw out_of_range("Wrong height of the board");
		break;
	}
	case s2i(SETTING_PLAYER_NAMES):
	{
		stringstream args(NextCmd());
		string player1, player2;
		getline(args, player1, ',');
		getline(args, player2, ',');
		bot->SetPlayerNames(player1, player1);
		break;
	}
	default:
		throw invalid_argument("Unknown setting");
	}
}	

