#include <cstdlib>
#include <exception>
#include <iostream>

#include "App/Game/Game.h"
#include "Resources/ProjectPaths.h"

int main(int, char* argv[])
{
	try
	{
		Game game(ProjectPaths::FromExecutable(argv[0]));
		game.Run();
	}
	catch (const std::exception& error)
	{
		std::cerr << "Fatal error: " << error.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
