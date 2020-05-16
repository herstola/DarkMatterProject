#include <iostream>
#include "SDL.h"
#include "Game.h"

int main(int argc, char** args)
{
	Game* game = Game::getInstance();
	game->play();
	return 0;
}