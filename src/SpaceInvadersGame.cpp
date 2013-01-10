/**
 * @file
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it like you want.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND COMES WITHOUT ANY
 * WARRANTIES
 *
 * @section DESCRIPTION
 *
 * 
 */

#include "K15_Engine/EventManager.h"

#include "HighscoreEnterState.h"
#include "SpaceInvadersGame.h"
#include "MainMenuState.h"

using namespace K15_SpaceInvaders;

void SpaceInvadersGame::start()
{
	ADD_LISTENER(this,Events::WindowClosed);
	getStateMachine()->setNextState(new MainMenuState());

	Game::start();
}

bool SpaceInvadersGame::handleEvent(const Event& gameEvent)
{
	stop();

	return true;
}

void SpaceInvadersGame::stop()
{
	Game::stop();
}