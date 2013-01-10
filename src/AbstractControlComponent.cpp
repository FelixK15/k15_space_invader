#include "AbstractControlComponent.h"

#include "MoveableComponent.h"

#include "K15_Engine/EventManager.h"

using namespace K15_SpaceInvaders;


AbstractControlComponent::AbstractControlComponent()
	: AbstractComponent("ControlComponent")
{
	m_keyLeft = 0;
	m_keyRight = 0;
	m_keyShot = 0;
	m_leftPressed = false;
	m_rightPressed = false;
}

void AbstractControlComponent::start()
{

}

AbstractComponent* AbstractControlComponent::clone()
{
	AbstractControlComponent *component = new AbstractControlComponent();
	
	component->m_keyLeft = m_keyLeft;
	component->m_keyRight = m_keyRight;
	component->m_keyShot = m_keyShot;

	return component;
}

void AbstractControlComponent::update( U32 deltaTime )
{
	Position pos = m_parent->getPosition();
	if(m_leftPressed){
		pos.setX(pos.getX() - static_cast<int>(0.8f * deltaTime));
	}else if(m_rightPressed){
		pos.setX(pos.getX() + static_cast<int>(0.8f * deltaTime));
	}
	m_parent->setPosition(pos);
}

bool AbstractControlComponent::handleMessage( const Event &gameEvent )
{
	if(gameEvent.getType() == Events::KeyboardPressed || gameEvent.getType() == Events::KeyboardReleased){
		int key = gameEvent.getArgument("key").toInteger;

		if(key == m_keyLeft || key == m_keyRight){
			if(gameEvent.getType() == Events::KeyboardPressed){
				if(key == m_keyLeft){
					m_leftPressed = true;
				}else{
					m_rightPressed = true;
				}
			}else{
				if(key == m_keyLeft){
					m_leftPressed = false;
				}else{
					m_rightPressed = false;
				}
			}
		}else if(key == m_keyShot){

			//Create ShotCreate Component
			Event newEvent(SI_Events::ShotCreated);
			newEvent.setArgument("ID",Variant(getParent()->getID()));
			EventManager::getInstance()->addEventToQueue(newEvent);
		}

		return true;
	}

	return false;
}

void AbstractControlComponent::shutdown()
{

}