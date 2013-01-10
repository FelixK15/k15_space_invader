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

#ifndef __SI_CONTROL_COMPONENT_TWO__
#define __SI_CONTROL_COMPONENT_TWO__

#include "StdInclude.h"

#include "K15_Engine/AbstractComponent.h"
#include "K15_Engine/IEventListener.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class ControlComponent_two : public AbstractComponent
	{

	typedef GameObject* Shot;

	public:

		ControlComponent_two();

		virtual void start() override;

		virtual AbstractComponent* clone() override;

		virtual void update(U32 deltaTime) override;

		virtual bool handleMessage(const Event &gameEvent ) override;

		virtual void shutdown() override;

	private:
		bool m_moveRight;
		bool m_moveLeft;
	};
}

#endif //__SI_CONTROL_COMPONENT_TWO__
