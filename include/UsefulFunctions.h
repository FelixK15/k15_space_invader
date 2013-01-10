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

#ifndef __HEADER_USEFUL_FUNCTIONS__
#define __HEADER_USEFUL_FUNCTIONS__

#include "StdInclude.h"

#define ARR_SET_TO_NULL(arr,length) for(int i = 0;i < length;++i){arr[i] = NULL;}
#define ARR_DELETE_ONE_BY_ONE(arr,length)for(int i = 0; i < length;++i){SAFE_DELETE(arr[i]);}

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	//Get a Random Number
	template<typename T>
	inline T getRandom(T min,T max)
	{
		U32 seed = static_cast<U32>(time(NULL));
		srand(seed);
		return static_cast<T>(rand() % max + min);
	}


	//Check for intersections.
	inline bool intersect(const Position& pos1,const Size& size1,const Position& pos2,const Size& size2)
	{
		bool intersect = false;

		if(pos1.getY() < pos2.getY() + size2.getHeight() && pos1.getY() + size1.getHeight() > pos2.getY()){
			if(pos1.getX() < pos2.getX() + size2.getWidth() && pos1.getX() + size1.getWidth() > pos2.getX()){
				intersect = true;
			}
		}

		return intersect;
	}
}

#endif //__HEADER_USEFUL_FUNCTIONS__