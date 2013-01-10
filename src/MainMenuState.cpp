#include "MainMenuState.h"
#include "GameObjectFactory.h"

#include "SpaceInvadersGame.h"

using namespace K15_SpaceInvaders;

MainMenuState::MainMenuState()
	: AbstractMenuState()
{
	_setMargin(10);
	_setMarginBetweenItems(20);

	//_setBackground(myGame->getGraphicManager()->createGraphic(Size(800,600),Position(0,0),Color::White));
	_addMenuItem(GameObjectFactory::createMenuItem("Start"));
	_addMenuItem(GameObjectFactory::createMenuItem("Start 2Player"));
	_addMenuItem(GameObjectFactory::createMenuItem("Show Highscore"));
	_addMenuItem(GameObjectFactory::createMenuItem("Quit"));
	_setMenuCenterPos(Position(400,300));
	_setMenuSize(Size(100,100));
}