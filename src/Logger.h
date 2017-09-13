#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Board.h"

class Logger
{
	static Logger* s_instance;

	std::string filename;
	Logger(const std::string& filename);
public:
	static void Initialize(const std::string& filename);
	static Logger* GetInstance();

	void PrintLn(const Board& board);
	void PrintLn(const std::string& line);
};

#ifdef LOGS_ENABLED
	#define LOG_INIT(arg) (Logger::Initialize(arg))
	#define LOG(arg) (Logger::GetInstance()->PrintLn(arg))
#else
	#define LOG(arg)
	#define LOG_INIT(arg)
#endif

#endif