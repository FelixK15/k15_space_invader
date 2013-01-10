#include "StdInclude.h"
#include "SpaceInvadersGame.h"

using namespace K15_SpaceInvaders;

int main(int argc,char *argv[])
{
	SpaceInvadersGame *newGame = new SpaceInvadersGame();

	newGame->initialize();
	newGame->start();
	newGame->shutdown();

	return 0;
}