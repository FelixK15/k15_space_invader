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

#include "ControlComponentPlayer2.h"

using namespace K15_SpaceInvaders;

ControlComponentPlayer2::ControlComponentPlayer2()
	: AbstractControlComponent()
{
	_setRightKey(K15_d);
	_setLeftKey(K15_a);
	_setShotKey(K15_w);
}