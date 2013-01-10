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

#ifndef __SI_EVENTS__
#define __SI_EVENTS__

#include "K15_Engine/Events.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class SI_Events
	{
	public:

		static const EventType ActivateItem;

		static const EventType ItemFocusGained;

		static const EventType ItemFocusLost;

		static const EventType RotateObject;

		static const EventType ShotCreated;

		static const EventType ShotHit;

		static const EventType EnemyHit;

		static const EventType PlayerHit;

		static const EventType BarricadeHit;

		static const EventType PowerUpCollected;

		static const EventType DamageTaken;

		static const EventType HealthRestored;

		static const EventType ChangeHorizontalDirection;
	};
	
}

#endif //__SI_EVENTS__