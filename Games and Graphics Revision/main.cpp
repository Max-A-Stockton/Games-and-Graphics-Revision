#include <iostream>

#define SDL_MAIN_HANDLED //Main handled by program as opposed to SDL. Prevents crash on startup.
#include "Game.h"

using namespace std;

Game game;

int main()
{
	bool initialized = game.Initialize();

	if (initialized)
	{
		game.GameLoop();
	}

	return 0;
}

