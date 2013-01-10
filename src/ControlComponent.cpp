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

#include "ControlComponent.h"

#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;

ControlComponent::ControlComponent()
	: AbstractComponent("ControlComponent")
{
	m_moveLeft = false;
	m_moveRight = false;
}

void ControlComponent::start()
{

}

AbstractComponent* ControlComponent::clone()
{
	return new ControlComponent();
}

void ControlComponent::update(U32 deltaTime)
{
	Position newPosition = getParent()->getPosition();

	if(m_moveLeft){
		newPosition.setX(newPosition.getX() - 4);
	}else if(m_moveRight){
		newPosition.setX(newPosition.getX() + 4);
	}

	getParent()->setPosition(newPosition);
}

bool ControlComponent::handleMessage( const Event &gameEvent )
{
	if(gameEvent.getType() == Events::KeyboardPressed || gameEvent.getType() == Events::KeyboardReleased){
		int key = gameEvent.getArgument("key").toInteger;

		if(key == K15_LEFT){
			if(gameEvent.getType() == Events::KeyboardPressed){
				m_moveLeft = true;
			}else{
				m_moveLeft = false;
			}
		}else if(key == K15_RIGHT){
			if(gameEvent.getType() == Events::KeyboardPressed){
				m_moveRight = true;
			}else{
				m_moveRight = false;
			}
		}else if(key == K15_SPACE){
			//shoot
			Event shotEvent(SI_Events::ShotCreated);
			shotEvent.setArgument("ID",Variant(m_parent->getID()));

			EventManager::getInstance()->triggerEvent(shotEvent);
		}

		return true;
	}

	return false;
}

void ControlComponent::shutdown()
{

}
