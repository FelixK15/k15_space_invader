#include "HighscoreEnterState.h"

#include "HighscoreState.h"

#include "K15_Engine/Game.h"
#include "K15_Engine/StringConverter.h"
#include "K15_Engine/EventManager.h"
#include "K15_Engine/Events.h"
#include "K15_Engine/Event.h"

using namespace K15_SpaceInvaders;

HighscoreEnterState::HighscoreEnterState(bool two_players,int ptP1,int ptP2)
{
	m_background = myGame->getGraphicManager()->createGraphic("background.png");
	m_twoPlayers = two_players;

	m_player1_ready = false;
	m_player2_ready = false;

	m_player1_points = ptP1;
	m_player2_points = ptP2;

	m_highScorePlayer1 = "Player one got " + StringConverter::toString(ptP1) + "Pts. Enter your Name:";

	if(!m_twoPlayers){
		m_player2_ready = true;
	}else{
		m_highScorePlayer2 = "Player two got " + StringConverter::toString(ptP2) + "Pts. Enter your Name:";
	}

	m_highScorePlayer1.setPosition(100,300);
	m_highScorePlayer2.setPosition(100,300);
	
	m_namePlayer1.setPosition(m_highScorePlayer1.getSize().getWidth() + 110,300);
	m_namePlayer2.setPosition(m_highScorePlayer2.getSize().getWidth() + 110,300);

	m_namePlayer1.setMaxSize(7);
	m_namePlayer2.setMaxSize(7);
}

bool HighscoreEnterState::start()
{
	ADD_LISTENER(this,Events::KeyboardPressed);
	return true;
}

void HighscoreEnterState::update(U32 deltaTime)
{
	DRAW_GRAPHIC(m_background);

	if(m_player1_ready && m_player2_ready){
		_saveHighscore();
		myGame->getStateMachine()->setNextState(new HighscoreState());
		myGame->getStateMachine()->removeCurState();
	}

	if(!m_twoPlayers){
		m_highScorePlayer1.update(deltaTime);
		m_namePlayer1.update(deltaTime);
	}else{
		if(!m_player1_ready){
			m_highScorePlayer1.update(deltaTime);
			m_namePlayer1.update(deltaTime);
		}else if(!m_player2_ready){
			m_highScorePlayer2.update(deltaTime);
			m_namePlayer2.update(deltaTime);
		}
	}
}

void HighscoreEnterState::end()
{
	REMOVE_LISTENER(this,Events::KeyboardPressed);
}

bool HighscoreEnterState::handleEvent(Event const &gameEvent)
{
	int key = gameEvent.getArgument("key").toInteger;

	if(key == K15_BACKSPACE){
		if(!m_player1_ready){
			m_namePlayer1.erase(m_namePlayer1.getLength(),1);	
		}else if(!m_player2_ready){
			m_namePlayer2.erase(m_namePlayer2.getLength(),1);	
		}
	}else if(!(key == K15_LSHIFT || key == K15_LCTRL || key == K15_LALT || key == K15_TAB || key == K15_RSHIFT)){
		if(key == K15_SPACE){
			key = '_';
		}else if(key == K15_ENTER){
			if(!m_player1_ready){
				m_player1_ready = true;
			}else if(!m_player2_ready){
				m_player2_ready = true;
			}
		}

		if(!m_twoPlayers && !m_player1_ready){
			m_namePlayer1 += key;
		}

		if(m_twoPlayers && m_player1_ready){
			m_namePlayer2 += key;
		}else{
			m_namePlayer1 += key;
		}
	}

	
	return true;
}

void HighscoreEnterState::_saveHighscore()
{
	std::ofstream stream("Highscore.dat",std::ios::app);
	if(stream.is_open()){
		stream << m_player1_points << ' ' << m_namePlayer1.getString()  << ' ';
		if(m_twoPlayers){
			stream << m_player2_points << ' ' <<  m_namePlayer2.getString()  << ' ';
		}

		stream.flush();
		stream.close();
	}
}
