#include "Logger.h";

#include <fstream>

using namespace std;

constexpr auto EMPTY = '.';
constexpr auto TAIL = 'x';

Logger* Logger::s_instance = nullptr;

void Logger::Initialize(const std::string& filename)
{
	if (!s_instance)
	{ 
		s_instance = new Logger(filename);
	}
}

Logger* Logger::GetInstance()
{
	return s_instance;
}

Logger::Logger(const std::string& filename)
	: filename(filename)
{	
	ofstream out(filename, ofstream::out);
}

void Logger::PrintLn(const Board& board)
{
	ofstream out(filename, ofstream::app);

	for (int j = 0; j < BOARD_SIZE; j++)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			bool empty_square = board.board[i][j];
			Position p(i, j);

			std::optional<int> player;
			for (int k = 0; k < board.pl_pos.size(); k++)
			{
				if (board.pl_pos[k] == p)
				{
					player = k;
					break;
				}
			}
			
			if (empty_square) out << EMPTY;
			else if (player.has_value()) out << player.value();
			else out << TAIL;
		}
		out << endl;
	}
	out.flush();
	out.close();
}

void Logger::PrintLn(const std::string& line)
{
	ofstream out(filename, ofstream::app);
	out << line << endl;
	out.flush();
	out.close();
}