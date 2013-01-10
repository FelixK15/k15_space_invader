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

#ifndef __SI_ABSTRACT_CONTROL_COMPONENT__
#define __SI_ABSTRACT_CONTROL_COMPONENT__

#include "StdInclude.h"

#include "K15_Engine/AbstractComponent.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class AbstractControlComponent : public AbstractComponent
	{
	public:

		virtual void start() override;

		virtual AbstractComponent* clone() override;

		virtual void update(U32 deltaTime) override;

		virtual bool handleMessage(const Event &gameEvent ) override;

		virtual void shutdown() override;

	protected:

		void _setLeftKey(S32 key){m_keyLeft = key;}

		void _setRightKey(S32 key){m_keyRight = key;}

		void _setShotKey(S32 key){m_keyShot = key;}

	private:
		bool m_leftPressed;
		bool m_rightPressed;

		S32 m_keyLeft;
		S32 m_keyRight;
		S32 m_keyShot;

	protected:
		AbstractControlComponent();

	};
}

#endif __SI_ABSTRACT_CONTROL_COMPONENT__
