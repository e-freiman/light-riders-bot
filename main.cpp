#include <memory>
#include <iostream>
#include "src/Parser.h"
#include "src/RandomBot.h"
#include "src/SnakeitBot.h"

using namespace std;

int main() 
try
{
	shared_ptr<Bot> bot = make_shared<SnakeitBot>();
	Parser parser(bot);
	parser.Parse();
	return 0;
}
catch (const std::exception& e)
{
	std::cout << e.what() << std::endl;
}
