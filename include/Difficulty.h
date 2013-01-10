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

#ifndef __SI_DIFFICULTY__
#define __SI_DIFFICULTY__

#include "StdInclude.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	struct Difficulty
	{
		float moveFactor;
		U32 timespanBetweenShots;
	};
}

#endif //__SI_DIFFICULTY__