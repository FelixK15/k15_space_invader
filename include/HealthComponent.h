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

#ifndef __SI_HEALTH_COMPONENT__
#define __SI_HEALTH_COMPONENT__

#include "StdInclude.h"
#include "K15_Engine/AbstractComponent.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class HealthComponent : public AbstractComponent
	{
	public:
		HealthComponent(U32 health);

		virtual AbstractComponent *clone() override {return new HealthComponent(m_originalHealth);}

		virtual bool handleMessage(const Event &gameEvent) override;

		void restoreHealth();

	private:
		void _updateAnimationBasedOnHealth();

		const U32 m_originalHealth;

		U32 m_health;
	};
}

#endif //__SI_HEALTH_COMPONENT__
