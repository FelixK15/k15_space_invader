#include "HighscoreState.h"

#include "MainMenuState.h"

#include "K15_Engine/Game.h"
#include "K15_Engine/StringConverter.h"
#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;

HighscoreState::HighscoreState()
{
	m_background = myGame->getGraphicManager()->createGraphic("background.png");
}

bool HighscoreState::start()
{
	ADD_LISTENER(this,Events::KeyboardPressed);
	_readHighscores();
	return true;
}

void HighscoreState::update(U32 deltaTime)
{
	DRAW_GRAPHIC(m_background);
	for(std::list<Highscore_entry>::iterator iter = m_highscores.begin();iter != m_highscores.end();iter++){
		(*iter).string.update(deltaTime);
	}
}

void HighscoreState::end()
{
	REMOVE_LISTENER(this,Events::KeyboardPressed);
}

bool HighscoreState::handleEvent(Event const &gameEvent)
{
	int key = gameEvent.getArgument("key").toInteger;
	if(key == K15_ESC){
		myGame->getStateMachine()->setNextState(new MainMenuState());
		myGame->getStateMachine()->removeCurState();
	}

	return true;
}

void HighscoreState::_readHighscores()
{
	std::ifstream stream("Highscore.dat");
	if(stream.is_open()){
		int i = 0;
		while(i < 10){
			int points;
			String name;

			stream >> points >> name; 
			
			if(name.empty()){
				points = 0;
			}
			
			m_highscores.push_back(Highscore_entry(DrawableString(StringConverter::toString(points) + "pts. - " + name),points));
			i++;
		}
	}

	int x = 200;
	int y = 200;

	int counter = 1;
	for(std::list<Highscore_entry>::iterator iter = m_highscores.begin();iter != m_highscores.end();iter++){
		String highscoreString = counter + ". " + (*iter).string.getString();
		(*iter).string = highscoreString;
		(*iter).string.setPosition(x,y);
		y+= 20;
		counter++;
	}

	m_highscores.sort();
	//m_highscores.reverse();
}
