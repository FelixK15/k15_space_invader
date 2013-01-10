#include "MenuState.h"

using namespace K15_SpaceInvaders;

MenuState::MenuState()
{
	_setMargin(10);
	_setMarginBetweenItems(5);

	_addMenuItem(GameObjectFactory::createMenuItem("Continue"));
	_addMenuItem(GameObjectFactory::createMenuItem("Quit"));

	_setMenuCenterPos(Position(400,300));
	_setMenuSize(Size(200,300));
}