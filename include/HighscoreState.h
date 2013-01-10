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

#ifndef __SI_HIGHSCORE_STATE__
#define __SI_HIGHSCORE_STATE__

#include "StdInclude.h"

#include "K15_Engine\DrawableString.h"
#include "K15_Engine\IState.h"
#include "K15_Engine\IEventListener.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class HighscoreState : public IState, public IEventListener
	{
	struct Highscore_entry{
		Highscore_entry(DrawableString string,int points){
			this->string = string;
			this->points = points;
		}

		DrawableString string;
		int points;

		bool operator<(const Highscore_entry &hsc){
			return this->points < hsc.points;
		}

		bool operator>(const Highscore_entry &hsc){
			return this->points > hsc.points;
		}
	};

	public:
		HighscoreState();

		virtual bool start() override;

		virtual bool handleEvent(Event const &gameEvent) override;

		virtual void update(U32 deltaTime) override;

		virtual void end() override;

	private:
		void _readHighscores();

		GraphicPtr m_background;
		std::list<Highscore_entry> m_highscores;
	};
}

#endif //__SI_HIGHSCORE_STATE__