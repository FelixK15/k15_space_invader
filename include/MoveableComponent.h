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

#ifndef __SI_MOVEABLE_COMPONENT__
#define __SI_MOVEABLE_COMPONENT__

#include "StdInclude.h"
#include "K15_Engine/AbstractComponent.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class MoveableComponent : public AbstractComponent
	{
	public:
		MoveableComponent(int velocityX, int velocityY);

		int getVelocityX(){return m_velocityX;}

		int getVelocityY(){return m_velocityY;}

		void setVelocityX(int velocityX){m_velocityX = velocityX;}

		void setVelocityY(int velocityY){m_velocityY = velocityY;}

		virtual AbstractComponent *clone() override;

		virtual void update(U32 deltaTime) override;

		virtual bool handleMessage(const Event &gameEvent ) override;

	private:
		int m_velocityX;
		int m_velocityY;

	};
}

#endif //__SI_MOVEABLE_COMPONENT__
