#include "PauseState.h"

#include "MainMenuState.h"

#include "K15_Engine/Game.h"
#include "K15_Engine/IGraphic.h"
#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;

PauseState::PauseState()
{
	m_backGround = myGame->getGraphicManager()->createGraphic("background.png");
	m_menuBackground = myGame->getGraphicManager()->createGraphic("pause_background.png");
	m_menuBackground->setColorkey(255,0,255);
	m_menuBackground->setPosition(200,150);
}

bool PauseState::start()
{
	ADD_LISTENER(this,Events::KeyboardPressed);
	return true;
}

void PauseState::update(U32 deltaTime)
{
	DRAW_GRAPHIC(m_backGround);
	DRAW_GRAPHIC(m_menuBackground);
}

bool PauseState::handleEvent(const Event& gameEvent)
{
	int key = gameEvent.getArgument("key").toInteger;

	if(key == K15_ESC){
		myGame->getStateMachine()->setNextState(new MainMenuState());
		myGame->getStateMachine()->removeCurState();
		myGame->getStateMachine()->removeCurState();
	}else if(key == K15_ENTER){
		myGame->getStateMachine()->removeCurState();
	}

	return true;
}

void PauseState::end()
{
	REMOVE_LISTENER(this,Events::KeyboardPressed);
}