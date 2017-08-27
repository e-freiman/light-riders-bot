#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <sstream>

#include "Bot.h"

class Parser 
{
public:
	Parser(std::shared_ptr<Bot> bot);
	void Parse();
private:
	std::shared_ptr<Bot> bot;
	
	void ProcessAction();
	void ProcessUpdate();
	void ProcessSettings();

	std::string NextCmd();
	std::stringstream cmdLine;
};

#endif
