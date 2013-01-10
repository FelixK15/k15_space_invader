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

#include "MoveableComponent.h"

using namespace K15_SpaceInvaders;

MoveableComponent::MoveableComponent(int velocityX, int velocityY)
	: AbstractComponent("MoveableComponent")
{
	m_velocityY = velocityY;
	m_velocityX = velocityX;
}

AbstractComponent* MoveableComponent::clone()
{
	return new MoveableComponent(m_velocityX,m_velocityY);
}

void MoveableComponent::update(U32 deltaTime)
{
	int newX = getParent()->getPosition().getX() + static_cast<int>((m_velocityX * 0.1f) * deltaTime);
	int newY = getParent()->getPosition().getY() + static_cast<int>((m_velocityY * 0.1f) * deltaTime);

	Position newPos(newX,newY);
	newPos.setX(newPos.getX() + m_velocityX);

	getParent()->setPosition(newPos);
}

bool MoveableComponent::handleMessage( const Event &gameEvent )
{
	if(gameEvent.getType() == SI_Events::ChangeHorizontalDirection){
		m_velocityX = -m_velocityX;
	}

	return true;
}
