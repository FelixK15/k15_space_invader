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


#include "HealthComponent.h"
#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;

HealthComponent::HealthComponent(U32 health)
	: AbstractComponent("HealthComponent"),
	  m_health(health),
	  m_originalHealth(health)
{

}

bool HealthComponent::handleMessage(const Event &gameEvent)
{
	if(gameEvent.getType() == SI_Events::DamageTaken){
		U32 ID = gameEvent.getArgument("ID").toInteger;
		if(ID == getParent()->getID()){
			m_health--;
			_updateAnimationBasedOnHealth();
		}

		return true;
	}

	return false;
}

void HealthComponent::restoreHealth()
{
	m_health = m_originalHealth;

	_updateAnimationBasedOnHealth();
}

void HealthComponent::_updateAnimationBasedOnHealth()
{
	if(m_health == 10 || m_health == 7 || m_health == 5 || m_health == 0){
		Event changeAnimationEvent(Events::NewAnimation);

		if(m_health == 10){
			changeAnimationEvent.setArgument("Name",Variant("normal"));
		}else if(m_health == 7){
			changeAnimationEvent.setArgument("Name",Variant("hurt"));
		}else if(m_health == 5){
			changeAnimationEvent.setArgument("Name",Variant("badly_hurt"));
		}else if(m_health == 0){
			changeAnimationEvent.setArgument("Name",Variant("dead"));
		}

		getParent()->broadcastMessage(changeAnimationEvent);
	}
}
