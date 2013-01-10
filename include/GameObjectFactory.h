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

#ifndef __SI_GAMEOBJECT_FACTORY__
#define __SI_GAMEOBJECT_FACTORY__

#include "K15_Engine/GameObject.h"

using namespace K15_Engine; 

namespace K15_SpaceInvaders
{
	class GameObjectFactory
	{
	public:

		static GameObject* createMenuItem(const String &type);

		static GameObject* createEnemy(const String &type,int factor);

		static GameObject* createPlayer(const String &type);

		static GameObject* createShot(const String& identifier,int velX,int velY);

		static GameObject* createExplosion(int x,int y);

		static GameObject* createBarrier();

		static GameObject* createPowerUp(int x,int y,int *kindOfPowerUp);

		static GameObject* createPoints(const Position &pos,int points);
	};
}

#endif //__SI_GAMEOBJECT_FACTORY__

