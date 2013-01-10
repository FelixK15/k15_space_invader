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

#ifndef __SI_PAUSE_STATE__
#define __SI_PAUSE_STATE__

#include "StdInclude.h"

#include "K15_Engine/IEventListener.h"
#include "K15_Engine/IState.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class PauseState : public IState, public IEventListener
	{
	public:
		PauseState();

		virtual bool start() override;

		virtual void update(U32 deltaTime) override;

		virtual void end() override;

		virtual bool handleEvent(const Event& gameEvent) override;

	private:
		GraphicPtr m_backGround;
		GraphicPtr m_menuBackground;
	};
}

#endif //__SI_PAUSE_STATE__