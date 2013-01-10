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

#include "ControlComponentPlayer1.h"

using namespace K15_SpaceInvaders;

ControlComponentPlayer1::ControlComponentPlayer1()
	: AbstractControlComponent()
{
	_setRightKey(K15_RIGHT);
	_setLeftKey(K15_LEFT);
	_setShotKey(K15_SPACE);
}