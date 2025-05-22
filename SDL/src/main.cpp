#include"Game.h"
#include <stdio.h>
#include <ctime>
int main(int argc, char* args[])
{
	srand(static_cast<unsigned int>(time(0)));
	//Start up SDL and create window
	Game::GetInstance()->Run();
	//Free resources and close SDL
	Game::GetInstance()->FreeInstance();

	return 0;
}