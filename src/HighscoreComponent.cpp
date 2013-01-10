#include "HighscoreComponent.h"

#include "K15_Engine/StringConverter.h"

using namespace K15_SpaceInvaders;

HighscoreComponent::HighscoreComponent()
	: AbstractComponent("HighscoreComponent")
{
	m_highscore = 0;
	m_graphicalHighscore = new DrawableString();
	m_graphicalHighscore->setFont("font.ttf",false);
	m_graphicalHighscore->setMargin(5);
	m_graphicalHighscore->setPosition(400,30);
}

HighscoreComponent::~HighscoreComponent()
{
	SAFE_DELETE(m_graphicalHighscore);
}

void HighscoreComponent::update( U32 gameTime )
{
	if(m_graphicalHighscore != NULL){
		String highscoreStr = "Highscore = " + StringConverter::toString(m_highscore);
		m_graphicalHighscore->operator=(highscoreStr);

		m_graphicalHighscore->update(gameTime);
	}
}