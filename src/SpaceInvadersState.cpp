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

#include "SpaceInvadersState.h"
#include "UsefulFunctions.h"
#include "HighscoreEnterState.h"
#include "PauseState.h"

#include "K15_Engine/AnimatedComponent.h"
#include "K15_Engine/EventManager.h"
#include "K15_Engine/CollisionManager.h"
#include "K15_Engine/GameObjectManager.h"
#include "K15_Engine/GameObject.h"
#include "K15_Engine/IGraphic.h"
#include "K15_Engine/Game.h"

using namespace K15_SpaceInvaders;

namespace{
	const int FASTER_PLAYER = 1;
	const int ONE_UP = 2;
	const int REPAIR_BARRIER = 3;
	const int POWER_SHOT = 4;
}

U32 SpaceInvadersState::DYNAMIC_UPPER_ALIEN_SPAWNTIME = SpaceInvadersState::STATIC_UPPER_ALIEN_SPAWNTIME;

bool SpaceInvadersState::PLAYER_ONE_DEAD = false;
bool SpaceInvadersState::PLAYER_TWO_DEAD = false;

U32 SpaceInvadersState::DYNAMIC_SPAWN_TIME_PLAYER1 = SpaceInvadersState::CONST_SPAWN_TIME_PLAYER_1;
U32 SpaceInvadersState::DYNAMIC_SPAWN_TIME_PLAYER2 = SpaceInvadersState::CONST_SPAWN_TIME_PLAYER_2;

bool SpaceInvadersState::POWERUP_ACTIVE = false;
U32 SpaceInvadersState::DYNAMIC_TIMER_POWERUP_FASTER_SHOT = 2000;
const U32 SpaceInvadersState::CONST_TIMER_POWERUP_FASTER_SHOT = 2000;
const F32 SpaceInvadersState::SHOT_MOVEFACTOR = 0.5f;


//Difficulties
const Difficulty SpaceInvadersState::EASY_DIFFICULTY = {1.0f,3000};
const Difficulty SpaceInvadersState::MEDIUM_DIFFICULTY = {1.5f,2000};
const Difficulty SpaceInvadersState::HARD_DIFFICULTY = {2.0f,1000};
const Difficulty SpaceInvadersState::GODLIKE_DIFFICULTY = {3.0f,550};

const F32 SpaceInvadersState::CONST_MOVE_FACTOR = 0.2f;

Difficulty SpaceInvadersState::m_curDifficulty = EASY_DIFFICULTY;

bool SpaceInvadersState::ENEMY_TOUCHED_BORDER = false;
U32 SpaceInvadersState::MILLISECONDS_BETWEEN_MOVEMENT = 200;
U32 SpaceInvadersState::TEMP_MILLISECONDS_BETWEEN_MOVEMENT = 200;
U32 SpaceInvadersState::TEMP_MILLISECONDS_BETWEEN_SHOTS = m_curDifficulty.timespanBetweenShots;
U32 SpaceInvadersState::DYNAMIC_AMOUNT_EMENIES = CONST_AMOUNT_ENEMIES;
F32 SpaceInvadersState::DYNAMIC_MOVE_FACTOR = 0.2f;

ENUM_ENEMY_DIRECTION SpaceInvadersState::ENEMY_DIRECTION = DIRECTION_LEFT;



bool SpaceInvadersState::start()
{
	srand(static_cast<int>(time(NULL)));

	ADD_LISTENER(this,SI_Events::ShotCreated);
	ADD_LISTENER(this,Events::KeyboardPressed);

	m_background = myGame->getGraphicManager()->createGraphic("background.png");

	DYNAMIC_MOVE_FACTOR = CONST_MOVE_FACTOR;

	m_upperAlien = NULL;
	m_shot = NULL;
	m_shot2 = NULL;
	m_curPowerup = -1;

	m_lives_p1 = 3;

	if(m_twoplayer){
		m_lives_p2 = 3;
	}else{
		m_lives_p2 = 0;
	}
	
	m_highscore_p1 = 0;
	m_highscore_p2 = 0;

	m_player1_info.setPosition(10,10);
	m_player2_info.setPosition(550,10);

	m_powerUp = NULL;

	m_player = GameObjectFactory::createPlayer("Player");
	m_player->setPosition(200,550);

	if(m_twoplayer){
		m_player2 = GameObjectFactory::createPlayer("Player2");
		m_player2->setPosition(400,550);
	}else{
		m_player2 = NULL;
	}

	_updatePlayerInfos();
	_createEnemies();
	_createBarriers();

	for(int i = 0;i < MAX_SHOTS;i++){
		m_enemyshots[i] = NULL;
	}

	return true;
}

void SpaceInvadersState::update(U32 deltaTime)
{
	if(m_lives_p1 == 0 && m_lives_p2 == 0){
		myGame->getStateMachine()->setNextState(new HighscoreEnterState(m_twoplayer,m_highscore_p1,m_highscore_p2));
		myGame->getStateMachine()->removeCurState();
	}

	DRAW_GRAPHIC(m_background);
	
	m_player1_info.update(deltaTime);
	m_player2_info.update(deltaTime);

	//Try to spawn a new shot from an enemy
	_spawnEnemyShot(deltaTime);

	//Update Power up
	_updatePowerUp(deltaTime);

	//Update player

	if(PLAYER_ONE_DEAD && m_lives_p1 > 0){
		if(DYNAMIC_SPAWN_TIME_PLAYER1 <= deltaTime){
			PLAYER_ONE_DEAD = false;
			m_player->setPosition(m_player->getPosition().getX(),550);
		}else{
			DYNAMIC_SPAWN_TIME_PLAYER1 -= deltaTime;
		}
	}

	if(PLAYER_TWO_DEAD && m_lives_p2 > 0){
		if(DYNAMIC_SPAWN_TIME_PLAYER2 <= deltaTime){
			PLAYER_TWO_DEAD = false;
			m_player2->setPosition(m_player2->getPosition().getX(),550);
		}else{
			DYNAMIC_SPAWN_TIME_PLAYER2 -= deltaTime;
		}
	}

	if(!PLAYER_ONE_DEAD && m_player){
		if(m_player->getPosition().getX() < 0){
			m_player->setPosition(0,m_player->getPosition().getY());
		}else if(m_player->getPosition().getX() + m_player->getSize().getWidth() > 800){
			m_player->setPosition(768,m_player->getPosition().getY());
		}
		m_player->update(deltaTime);
	}

	if(!PLAYER_TWO_DEAD &&m_player2){
		if(m_player2->getPosition().getX() < 0){
			m_player2->setPosition(0,m_player2->getPosition().getY());
		}else if(m_player2->getPosition().getX() + m_player2->getSize().getWidth() > 800){
			m_player2->setPosition(768,m_player2->getPosition().getY());
		}
		m_player2->update(deltaTime);
	}


	if(m_upperAlien != NULL){
		if(m_shot){
			if(intersect(m_shot->getPosition(),m_shot->getSize(),m_upperAlien->getPosition(),m_upperAlien->getSize())){
				_createExplosion(m_upperAlien->getPosition());
				_showPoints(m_upperAlien->getPosition(),200);
				SAFE_DELETE(m_upperAlien);
				SAFE_DELETE(m_shot);
				m_highscore_p1 += 200;
				_updatePlayerInfos();
			}
		}else if(m_shot2){

			if(intersect(m_shot2->getPosition(),m_shot2->getSize(),m_upperAlien->getPosition(),m_upperAlien->getSize())){
				_createExplosion(m_upperAlien->getPosition());
				_showPoints(m_upperAlien->getPosition(),200);
				SAFE_DELETE(m_upperAlien);
				SAFE_DELETE(m_shot2);
				m_highscore_p2 += 200;
				_updatePlayerInfos();
			}

		}

		if(m_upperAlien != NULL){
			m_upperAlien->update(deltaTime);
			Position pos = m_upperAlien->getPosition();
			if(pos.getX() <= 0){
				SAFE_DELETE(m_upperAlien);
				DYNAMIC_UPPER_ALIEN_SPAWNTIME = STATIC_UPPER_ALIEN_SPAWNTIME;
			}else{
				pos.setX(pos.getX() - static_cast<int>(deltaTime * CONST_MOVE_FACTOR));
				m_upperAlien->setPosition(pos);
			}
		}		
	}

	if(DYNAMIC_UPPER_ALIEN_SPAWNTIME < deltaTime){
		_spawnUpperAlien();
	}else{
		DYNAMIC_UPPER_ALIEN_SPAWNTIME -= deltaTime;
	}

	//Update Player Shot
	_updatePlayerShot(deltaTime);

	//Update Enemies
	_updateEnemies(deltaTime);

	//Update barriers
	_updateBarriers(deltaTime);

	//Update enemy shots
	_updateShots(deltaTime);

	//Update the explosions.
	_updateExplosions(deltaTime);

	_updatePoints(deltaTime);

	if(POWERUP_ACTIVE){
		if(DYNAMIC_TIMER_POWERUP_FASTER_SHOT < deltaTime){
			POWERUP_ACTIVE = false;
			m_curPowerup = -1;
		}else{
			DYNAMIC_TIMER_POWERUP_FASTER_SHOT -= deltaTime;
		}
	}

	if(ENEMY_TOUCHED_BORDER) {
		//Update position of all enemies if one of them touched the border.
		for(int i = 0;i < CONST_AMOUNT_ENEMIES;i++){
			if(m_enemies[i] != NULL){
				m_enemies[i]->setPosition(m_enemies[i]->getPosition().getX(),m_enemies[i]->getPosition().getY() + 10);
				if(m_enemies[i]->getPosition().getY() + m_enemies[i]->getSize().getHeight() >= 600){

				}
			}
		}

		ENEMY_TOUCHED_BORDER = false; 
		ENEMY_DIRECTION = ENEMY_DIRECTION == DIRECTION_LEFT ? DIRECTION_RIGHT : DIRECTION_LEFT;
	}
}

void SpaceInvadersState::end()
{
	REMOVE_LISTENER(this,SI_Events::ShotCreated);
	REMOVE_LISTENER(this,Events::KeyboardPressed);

	for(int i = 0;i < CONST_AMOUNT_ENEMIES;i++){
		SAFE_DELETE(m_enemies[i]);
	}

	for(int i = 0;i < AMOUNT_BARRIERS;i++){
		SAFE_DELETE(m_barriers[i].m_barrier);
	}

	for(int i = 0;i < MAX_EXPLOSIONS;i++){
		SAFE_DELETE(m_explosions[i].m_explosion);
	}

	for(int i = 0;i < MAX_SHOTS;i++){
		SAFE_DELETE(m_enemyshots[i]);
		SAFE_DELETE(m_points[i].m_points)
	}

	SAFE_DELETE(m_powerUp);

	SAFE_DELETE(m_shot);
	SAFE_DELETE(m_shot2);

	SAFE_DELETE(m_player);
	SAFE_DELETE(m_player2);
}

void SpaceInvadersState::setShot( Shot shot ,int player)
{
	if(player == 1){
		if(!shot){
			return;
		}

		if(m_shot){
			SAFE_DELETE(shot);
		}else{
			m_shot = shot;
		}
	}else{
		if(!shot){
			return;
		}

		if(m_shot2){
			SAFE_DELETE(shot);
		}else{
			m_shot2 = shot;
		}
	}
	
}

bool SpaceInvadersState::handleEvent( const Event& gameEvent )
{
	if(gameEvent.getType() == Events::KeyboardPressed){
		int key = gameEvent.getArgument("key").toInteger;
		if(key == K15_ESC){
			myGame->getStateMachine()->setNextState(new PauseState());

		}
	}else if(gameEvent.getType() == SI_Events::ShotCreated){
		U32 ID = gameEvent.getArgument("ID").toInteger;
		if(m_player){
			if(m_player->getID() == ID){
				Shot newShot = GameObjectFactory::createShot("player_shot",0,-2);
				newShot->setPosition(m_player->getPosition());
				setShot(newShot,1);
			}
		}

		if(m_player2){
			if(m_player2->getID() == ID){
				Shot newShot = GameObjectFactory::createShot("player_shot",0,-2);
				newShot->setPosition(m_player2->getPosition());
				setShot(newShot,2);
			}
		}


		return true;
	}

	return false;
}

void SpaceInvadersState::_createEnemies()
{
	int x = 150;
	int y = 60;

	DYNAMIC_AMOUNT_EMENIES = CONST_AMOUNT_ENEMIES;

	for(int i = 0;i < CONST_AMOUNT_ENEMIES;i++){
		m_enemies[i] = GameObjectFactory::createEnemy("Enemy",i);
		m_enemies[i]->setPosition(x,y);
		x += 50;
		if(x > 650){
			y+= 50;
			x = 150;
		}
	}
}

void SpaceInvadersState::_createBarriers()
{
	int x = 100;
	int y = 450;

	for(int i = 0;i < AMOUNT_BARRIERS;i++){
		m_barriers[i].m_barrier = GameObjectFactory::createBarrier();
		m_barriers[i].m_health = 10;
		m_barriers[i].m_barrier->setPosition(x,y);
		x += 300;
	}
}

void SpaceInvadersState::_createExplosion(const Position& pos)
{
	for(int j = 0;j < MAX_EXPLOSIONS;j++){
		if(m_explosions[j].m_explosion == NULL){
			m_explosions[j].m_explosion = GameObjectFactory::createExplosion(pos.getX(),pos.getY());
			m_explosions[j].m_lifespan = 320;

			break;
		}	
	}
}

void SpaceInvadersState::_updateExplosions( U32 deltaTime )
{
	for(int i = 0;i < MAX_EXPLOSIONS;i++){
		if(m_explosions[i].m_explosion != NULL){
			int difference = m_explosions[i].m_lifespan - deltaTime;
			if(difference <= 0){
				SAFE_DELETE(m_explosions[i].m_explosion);
			}else{
				m_explosions[i].m_lifespan -= deltaTime;
				m_explosions[i].m_explosion->update(deltaTime);
			}
		}
	}
}

void SpaceInvadersState::_updateEnemies( U32 deltaTime )
{
	bool changePos = false;

	U32 moveTime = (U32)(deltaTime * (m_curDifficulty.moveFactor + DYNAMIC_MOVE_FACTOR)); 

	if(moveTime > TEMP_MILLISECONDS_BETWEEN_MOVEMENT){
		TEMP_MILLISECONDS_BETWEEN_MOVEMENT = MILLISECONDS_BETWEEN_MOVEMENT;
		changePos = true;
	}else{
		TEMP_MILLISECONDS_BETWEEN_MOVEMENT -= moveTime;
	}

	for(int i = 0;i < CONST_AMOUNT_ENEMIES;i++){
		if(m_enemies[i] != NULL){

			if(m_enemies[i]->getPosition().getY() + m_enemies[i]->getSize().getHeight() > 600){
				myGame->getStateMachine()->setNextState(new HighscoreEnterState(m_twoplayer,m_highscore_p1,m_highscore_p2));
				myGame->getStateMachine()->removeCurState();
			}

			if(m_shot != NULL){

				//Check for intersection
				Position pos_shot = m_shot->getPosition();
				Position pos_enemy = m_enemies[i]->getPosition();
				Size size_shot = m_shot->getSize();
				Size size_enemy = m_enemies[i]->getSize();

				if(intersect(pos_shot,size_shot,pos_enemy,size_enemy)){
					DYNAMIC_MOVE_FACTOR+= 0.15f;
					//If an intersection (or a collision, if you like) happened, the shot and the corrospending enemy will get deleted.
					SAFE_DELETE(m_enemies[i]);
					SAFE_DELETE(m_shot);

					_showPoints(pos_enemy,100);
					m_highscore_p1 += 100;

					_updatePlayerInfos();

					--DYNAMIC_AMOUNT_EMENIES;

					//Check if all enemies are dead.
					if(DYNAMIC_AMOUNT_EMENIES == 0){
						_startNewRound();
					}

					//Shall we spawn a power up?//
					if(getRandom<int>(0,4) == 3){
						_spawnPowerUp(pos_enemy);
					}

					//Create a new explosion at the enemies position.
					_createExplosion(pos_enemy);

					//The enemy got deleted so there's no reason in updating its position.
					continue;
				}
			}

			if(m_shot2 != NULL){

				//Check for intersection
				Position pos_shot = m_shot2->getPosition();
				Position pos_enemy = m_enemies[i]->getPosition();
				Size size_shot = m_shot2->getSize();
				Size size_enemy = m_enemies[i]->getSize();

				if(intersect(pos_shot,size_shot,pos_enemy,size_enemy)){
					DYNAMIC_MOVE_FACTOR+= 0.15f;
					//If an intersection (or a collision, if you like) happened, the shot and the corrospending enemy will get deleted.
					SAFE_DELETE(m_enemies[i]);
					SAFE_DELETE(m_shot2);

					_showPoints(pos_enemy,100);
					m_highscore_p2 += 100;
					
					_updatePlayerInfos();

					--DYNAMIC_AMOUNT_EMENIES;

					//Check if all enemies are dead.
					if(DYNAMIC_AMOUNT_EMENIES == 0){
						_startNewRound();
					}

					//Shall we spawn a power up?//
					if(getRandom<int>(0,4) == 3){
						_spawnPowerUp(pos_enemy);
					}

					//Create a new explosion at the enemies position.
					_createExplosion(pos_enemy);

					//The enemy got deleted so there's no reason in updating its position.
					continue;
				}
			}

			for(int j = 0;j < AMOUNT_BARRIERS;j++){
				if(m_barriers[j].m_health > 0){
					const Position &pos_barrier = m_barriers[j].m_barrier->getPosition();
					const Size &size_barrier = m_barriers[j].m_barrier->getSize();
					const Position &pos_enemy = m_enemies[i]->getPosition();
					const Size &size_enemy = m_enemies[i]->getSize();

					if(intersect(pos_enemy,size_enemy,pos_barrier,size_barrier)){
						m_barriers[j].m_health = 0;
					}
				}
			}

			//Check if the position needs to get change.d
			if(changePos){

				//Calculate new position.
				Position oldPosition = m_enemies[i]->getPosition();
				int newX = 0;
				int newY = 0;

				newY = oldPosition.getY();
				if(ENEMY_DIRECTION == DIRECTION_RIGHT){
					newX = oldPosition.getX() + 10;
				}else{
					newX = oldPosition.getX() - 10;
				}

				//Update Position.
				m_enemies[i]->setPosition(newX,newY);
			}

			//Update Enemy
			m_enemies[i]->update(deltaTime);


			//Check if the enemy is at the border of the screen.
			Size enemySize = m_enemies[i]->getSize();
			Position enemyPos = m_enemies[i]->getPosition();
			if(!ENEMY_TOUCHED_BORDER){
				//If the enemy is at the border of the screen, the flag for border collision will get set.
				if(ENEMY_DIRECTION == DIRECTION_LEFT){
					if(enemyPos.getX() <= 0){
						ENEMY_TOUCHED_BORDER = true;
					}
				}else{
					if(enemyPos.getX() + enemySize.getWidth() > 800){
						ENEMY_TOUCHED_BORDER = true;
					}
				}
			}
		}	
	}
}

void SpaceInvadersState::_updateShots( U32 deltaTime )
{
	for(int i = 0;i < MAX_SHOTS;i++){
		if(m_enemyshots[i] != NULL){
			m_enemyshots[i]->update(deltaTime);

			if(m_enemyshots[i]->getPosition().getY() > 600){
				SAFE_DELETE(m_enemyshots[i]);

				//Skip the rest if the shot got deleted.
				continue;
			}

			//Check if a shot intersected with the player.
			Position shot_position = m_enemyshots[i]->getPosition();
			Size shot_size = m_enemyshots[i]->getSize();

			Position player_position = m_player->getPosition();
			Size player_size = m_player->getSize();

			if(m_player2){
				Position player_position = m_player2->getPosition();
				Size player_size = m_player2->getSize();

				if(intersect(shot_position,shot_size,player_position,player_size)){
					//Create a new explosion at the players position.
					_createExplosion(player_position);
					m_lives_p2 -= 1;
					PLAYER_TWO_DEAD = true;
					m_player2->setPosition(m_player->getPosition().getX(),0);

					_updatePlayerInfos();

					if(m_lives_p2 != 0){
						DYNAMIC_SPAWN_TIME_PLAYER2 = CONST_SPAWN_TIME_PLAYER_2;
					}		
					
				}
			}

			if(intersect(shot_position,shot_size,player_position,player_size)){
				//Create a new explosion at the players position.
				_createExplosion(player_position);
				m_lives_p1 -= 1;
				PLAYER_ONE_DEAD = true;
				m_player->setPosition(m_player->getPosition().getX(),0);

				_updatePlayerInfos();

				if(m_lives_p1 != 0){
					DYNAMIC_SPAWN_TIME_PLAYER1 = CONST_SPAWN_TIME_PLAYER_1;
				}				
			}

			for(int j = 0;j < AMOUNT_BARRIERS;j++){
				if(m_barriers[j].m_health > 0){
					Position barrier_position = m_barriers[j].m_barrier->getPosition();
					Size barrier_size = m_barriers[j].m_barrier->getSize();


					bool intersection = false;
					//Did one of the shots (enemy as well as player shot) intersect with one of the barriers?
					if(intersect(shot_position,shot_size,barrier_position,barrier_size)){

						_createExplosion(shot_position);
						SAFE_DELETE(m_enemyshots[i]);
						intersection = true;
					}else if(m_shot != NULL && intersect(m_shot->getPosition(),m_shot->getSize(),barrier_position,barrier_size)){
						_createExplosion(m_shot->getPosition());
						SAFE_DELETE(m_shot);
						intersection = true;
					}else if(m_shot2 != NULL && intersect(m_shot2->getPosition(),m_shot2->getSize(),barrier_position,barrier_size)){
						_createExplosion(m_shot2->getPosition());
						SAFE_DELETE(m_shot2);
						intersection = true;
					}

					if(intersection){
						m_barriers[j].m_health--;
					}
				}
			}
		}
	}
}

void SpaceInvadersState::_updateBarriers( U32 deltaTime )
{
	for(int i = 0;i < AMOUNT_BARRIERS;i++){
		if(m_barriers[i].m_barrier != NULL){
			m_barriers[i].m_barrier->update(deltaTime);

			AnimatedComponent *animatedcomponent_barrier = m_barriers[i].m_barrier->getComponent<AnimatedComponent>("AnimatedComponent");

			//Changed the picture of the barrier based on its health.
			if(animatedcomponent_barrier != NULL){
				if(m_barriers[i].m_health == 10){
					animatedcomponent_barrier->startAnimation("normal");
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(10,0));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(2,10));
					m_barriers[i].m_health--;
				}else if(m_barriers[i].m_health == 7){
					animatedcomponent_barrier->startAnimation("hurt");
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(10,0));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(2,10));
					m_barriers[i].m_health--;
				}else if(m_barriers[i].m_health == 4){
					animatedcomponent_barrier->startAnimation("badly_hurt");
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(20,2));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(15,10));
					m_barriers[i].m_health--;
				}else if(m_barriers[i].m_health == 0){
					animatedcomponent_barrier->startAnimation("dead");
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(10,0));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(2,10));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(20,2));
					_createExplosion(m_barriers[i].m_barrier->getPosition() + Position(15,10));
					m_barriers[i].m_health--;
				}
			}
		}
	}
}

void SpaceInvadersState::_spawnEnemyShot( U32 deltaTime )
{
	//Check if an enemy is allowed to create a shot.
	if(deltaTime > TEMP_MILLISECONDS_BETWEEN_SHOTS){
		TEMP_MILLISECONDS_BETWEEN_SHOTS = m_curDifficulty.timespanBetweenShots;
		int enemyID = getRandom<int>(0,CONST_AMOUNT_ENEMIES);
		Enemy enemy = NULL;

		if(m_enemies[enemyID] != NULL){
			enemy = m_enemies[enemyID];
		}else{
			for(int i = 0;i < CONST_AMOUNT_ENEMIES;i++){
				if(m_enemies[i] != NULL){
					enemy = m_enemies[i];
				}
			}
		}

		if(enemy != NULL){
			for(int i = 0;i < MAX_SHOTS;i++){
				if(m_enemyshots[i] == NULL){
					m_enemyshots[i] = GameObjectFactory::createShot("EnemyShot",0,2);
					m_enemyshots[i]->setPosition(enemy->getPosition());
					break;
				}
			}
		}

	}else{
		TEMP_MILLISECONDS_BETWEEN_SHOTS -= deltaTime;
	}
}

void SpaceInvadersState::_updatePlayerShot( U32 deltaTime )
{
	//Check if the player shot is within the screen bounds.
 	if(m_shot != NULL){
		if(POWERUP_ACTIVE){
			if(m_curPowerup == FASTER_PLAYER){
				Position pos = m_shot->getPosition();
				pos.setY(pos.getY() - static_cast<int>(SHOT_MOVEFACTOR * deltaTime));
				m_shot->setPosition(pos);
			}
		}

		if(m_shot->getPosition().getY() < 0){
			SAFE_DELETE(m_shot);
		}else{
			m_shot->update(deltaTime);
		}
	}

	if(m_shot2 != NULL){
		if(POWERUP_ACTIVE){
			if(m_curPowerup == FASTER_PLAYER){
				Position pos = m_shot2->getPosition();
				pos.setY(pos.getY() - static_cast<int>(SHOT_MOVEFACTOR * deltaTime));
				m_shot2->setPosition(pos);
			}
		}

		if(m_shot2->getPosition().getY() < 0){
			SAFE_DELETE(m_shot2);
		}else{
			m_shot2->update(deltaTime);
		}
	}
}

void SpaceInvadersState::_spawnPowerUp(const Position& pos)
{
	//If theres already a powerup, go home.
	if(m_powerUp != NULL){
		return;
	}

	m_powerUp = GameObjectFactory::createPowerUp(pos.getX(),pos.getY(),&m_curPowerup);
}

void SpaceInvadersState::_updatePowerUp( U32 deltaTime )
{
	if(m_powerUp != NULL){
		Position powerup_pos = m_powerUp->getPosition();
		Size powerup_size = m_powerUp->getSize();

		Position player_pos = m_player->getPosition();
		Size player_size = m_player->getSize();

		if(m_player2){
			Position player2_pos = m_player2->getPosition();
			Size player2_size = m_player2->getSize();

			if(intersect(powerup_pos,powerup_size,player2_pos,player2_size)){
				POWERUP_ACTIVE = true;

				if(m_curPowerup == FASTER_PLAYER){
					DYNAMIC_TIMER_POWERUP_FASTER_SHOT = CONST_TIMER_POWERUP_FASTER_SHOT;
				}else if(m_curPowerup == REPAIR_BARRIER){
					_refreshBarriers();
				}else if(m_curPowerup == ONE_UP){
					m_lives_p2++;
				}

				SAFE_DELETE(m_powerUp);

				return;
			}
		}

		if(intersect(powerup_pos,powerup_size,player_pos,player_size)){

			POWERUP_ACTIVE = true;

			if(m_curPowerup == FASTER_PLAYER){
				DYNAMIC_TIMER_POWERUP_FASTER_SHOT = CONST_TIMER_POWERUP_FASTER_SHOT;
			}else if(m_curPowerup == REPAIR_BARRIER){
				_refreshBarriers();
			}else if(m_curPowerup == ONE_UP){
				m_lives_p1++;
			}

			SAFE_DELETE(m_powerUp);
		}else if(powerup_pos.getY() > 600){
			SAFE_DELETE(m_powerUp);
		}else{
			powerup_pos.setY(powerup_pos.getY() + (int)(CONST_MOVE_FACTOR * deltaTime));

			m_powerUp->setPosition(powerup_pos);
			m_powerUp->update(deltaTime);
		}
	}
}

void SpaceInvadersState::_startNewRound()
{
	_createEnemies();
	DYNAMIC_AMOUNT_EMENIES = CONST_AMOUNT_ENEMIES;
	m_curDifficulty.moveFactor += CONST_MOVE_FACTOR;
	m_curDifficulty.timespanBetweenShots += 200;
	DYNAMIC_MOVE_FACTOR = CONST_MOVE_FACTOR;
}

void SpaceInvadersState::_spawnUpperAlien()
{
	if(m_upperAlien == NULL){
		m_upperAlien = GameObjectFactory::createEnemy("Alien",0);
		m_upperAlien->setPosition(800,20);

		DYNAMIC_UPPER_ALIEN_SPAWNTIME = STATIC_UPPER_ALIEN_SPAWNTIME;
	}
}

void SpaceInvadersState::_refreshBarriers()
{
	for(int i = 0;i < AMOUNT_BARRIERS;i++){
		m_barriers[i].m_health = 10;
	}
}

void SpaceInvadersState::_showPoints( const Position& pos,int points )
{
	for(int i = 0;i < MAX_SHOTS;i++){
		if(m_points[i].m_points == NULL){
			m_points[i].m_points = GameObjectFactory::createPoints(pos,points);
			m_points[i].m_lifespan = 700;
			break;
		}
	}
}

void SpaceInvadersState::_updatePoints( U32 deltaTime )
{
	for(int i = 0;i < MAX_SHOTS;i++){
		if(m_points[i].m_points != NULL){
			if(m_points[i].m_lifespan < deltaTime){
				SAFE_DELETE(m_points[i].m_points);
			}else{
				m_points[i].m_points->update(deltaTime);
				m_points[i].m_lifespan -= deltaTime;
			}
		}
	}
}

void SpaceInvadersState::_updatePlayerInfos()
{
	if(m_player){
		if(m_lives_p1 == 0){
			m_player1_info = "Player 1 : " + StringConverter::toString(m_highscore_p1) + " Pt. | Lives : " + StringConverter::toString(m_lives_p1) + " DEAD";
		}else{
			m_player1_info = "Player 1 : " + StringConverter::toString(m_highscore_p1) + " Pt. | Lives : " + StringConverter::toString(m_lives_p1);
		}
	}
	
	if(m_player2){
		if(m_lives_p2 == 0){
			m_player2_info = "Player 2 : " + StringConverter::toString(m_highscore_p2) + " Pt. | Lives : " + StringConverter::toString(m_lives_p2) + " DEAD";
		}else{
			m_player2_info = "Player 2 : " + StringConverter::toString(m_highscore_p2) + " Pt. | Lives : " + StringConverter::toString(m_lives_p2);
		}
	}
}
