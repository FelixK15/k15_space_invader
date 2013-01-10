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

#include "SI_Events.h"

using namespace K15_SpaceInvaders;

const EventType SI_Events::ActivateItem = EventType("activate_item");

const EventType SI_Events::ItemFocusGained = EventType("item_focus_gained");

const EventType SI_Events::ItemFocusLost = EventType("item_focus_lost");

const EventType SI_Events::RotateObject = EventType("rotate_object");

const EventType SI_Events::ShotCreated = EventType("shot_created");

const EventType SI_Events::ShotHit = EventType("shot_hit");

const EventType SI_Events::EnemyHit = EventType("enemy_hit");

const EventType SI_Events::PlayerHit = EventType("player_hit");

const EventType SI_Events::BarricadeHit = EventType("barricade_hit");

const EventType SI_Events::PowerUpCollected = EventType("powerup_collected");

const EventType SI_Events::DamageTaken = EventType("damage_taken");

const EventType SI_Events::HealthRestored = EventType("health_restored");

const EventType SI_Events::ChangeHorizontalDirection = EventType("change_horizontal_direction");
