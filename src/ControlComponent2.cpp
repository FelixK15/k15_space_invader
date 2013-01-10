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

#include "ControlComponent2.h"

#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;

ControlComponent_two::ControlComponent_two()
	: AbstractComponent("ControlComponent2")
{
	m_moveLeft = false;
	m_moveRight = false;
}

void ControlComponent_two::start()
{

}

AbstractComponent* ControlComponent_two::clone()
{
	return new ControlComponent_two();
}

void ControlComponent_two::update(U32 deltaTime)
{
	Position newPosition = getParent()->getPosition();

	if(m_moveLeft){
		newPosition.setX(newPosition.getX() - 4);
	}else if(m_moveRight){
		newPosition.setX(newPosition.getX() + 4);
	}

	getParent()->setPosition(newPosition);
}

bool ControlComponent_two::handleMessage( const Event &gameEvent )
{
	if(gameEvent.getType() == Events::KeyboardPressed || gameEvent.getType() == Events::KeyboardReleased){
		int key = gameEvent.getArgument("key").toInteger;

		if(key == K15_a){
			if(gameEvent.getType() == Events::KeyboardPressed){
				m_moveLeft = true;
			}else{
				m_moveLeft = false;
			}
		}else if(key == K15_d){
			if(gameEvent.getType() == Events::KeyboardPressed){
				m_moveRight = true;
			}else{
				m_moveRight = false;
			}
		}else if(key == K15_w){
			//shoot
			Event shotEvent(SI_Events::ShotCreated);
			shotEvent.setArgument("ID",Variant(m_parent->getID()));

			EventManager::getInstance()->triggerEvent(shotEvent);
		}

		return true;
	}

	return false;
}

void ControlComponent_two::shutdown()
{

}
