#include "GameLogic.hpp"

/**
 * entry point
 */
int main(int argc, const char* argv[])
{
	GameLogic* game = new GameLogic(argc, argv, "Lost Kingdom", /*1920, 1080*/ 1280, 720, 60, 60);
	
	game->run();
	
	delete game;
	
	return 0;
}