#include <cstdlib>
#include <exception>
#include <iostream>

#include "App/Game.h"

int main()
{
	try
	{
		Game game;
		game.Run();
	}
	catch (const std::exception& error)
	{
		std::cerr << "Fatal error: " << error.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}