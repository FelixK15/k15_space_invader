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

#ifndef __SI_HIGHSCORE_COMPONENT__
#define __SI_HIGHSCORE_COMPONENT__

#include "StdInclude.h"

#include "K15_Engine/AbstractComponent.h"
#include "K15_Engine/DrawableString.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class HighscoreComponent : public AbstractComponent
	{
	public:
		HighscoreComponent();

		virtual ~HighscoreComponent();

		void addHighscore(U32 points){ m_highscore += points; }

		U32 getHighscore() { return m_highscore; }

		virtual AbstractComponent* clone() override {return new HighscoreComponent(*this);}

		virtual void update(U32 gameTime) override;

		virtual bool handleMessage(const Event &gameEvent ) override{return false;}

	private:
		U32 m_highscore;

		DrawableString *m_graphicalHighscore;
	};
}

#endif //__SI_HIGHSCORE_COMPONENT__