#include "SpaceInvaderState.h"
#include "GameObjectFactory.h"
#include "UsefulFunctions.h"
#include "MenuState.h"
#include "SI_Events.h"


#include "K15_Engine/Game.h"
#include "K15_Engine/GameObject.h"
#include "K15_Engine/CollisionManager.h"
#include "K15_Engine/EventManager.h"
#include "K15_Engine/Events.h"
#include "K15_Engine/Keymap.h"

using namespace K15_SpaceInvaders;

//Anonymous namespace = global for this file//
namespace{
	static const int POWERSHOT_FLAG = 0x00F;
	static const int PLAYERSHOT_FLAG = 0x0F0;
	static const int ENEMYSHOT_FLAG = 0xF00;
}
//////////////////////////////////////////////

//Static initialization//
const F32 SpaceInvadersState::CONST_MOVE_FACTOR = 0.2f;
F32 SpaceInvadersState::MOVE_FACTOR = SpaceInvadersState::CONST_MOVE_FACTOR;
U32 SpaceInvadersState::SHOT_TIMER = SpaceInvadersState::CONST_SHOT_TIMER;
/////////////////////////


//Typedefs//
typedef GameObject* Shot;
typedef GameObject* Enemy;
typedef GameObject* Player;
typedef GameObject* PowerUp;
typedef GameObject* Barricade;
////////////


//Collsion Handler//
void collision_handler_enemy__playershot(GameObject *go1,GameObject *go2)
{
	Shot shot = NULL;
	Enemy enemy = NULL;

	if(go1->getType() == "PlayerShot"){
		shot = go1;
		enemy = go2;
	}else{
		enemy = go1;
		shot = go2;
	}

	Event enemyHitEvent(SI_Events::EnemyHit);
	Event shotHitEvent(SI_Events::ShotHit);

	enemyHitEvent.setArgument("ID",Variant(enemy->getID()));
	shotHitEvent.setArgument("ID",Variant(shot->getID()));

	EventManager::getInstance()->addEventToQueue(enemyHitEvent);
	EventManager::getInstance()->addEventToQueue(shotHitEvent);

	return;
}

void collision_handler_enemyshot__player(GameObject *go1,GameObject *go2)
{
	Shot shot = NULL;
	Player player = NULL;

	if(go1->getType() == "Player"){
		player = go1;
		shot = go2;
	}else{
		shot = go1;
		player = go2;
	}

	Event shotHitEvent(SI_Events::ShotHit);
	Event playerHitEvent(SI_Events::PlayerHit);

	shotHitEvent.setArgument("ID",Variant(shot->getID()));
	playerHitEvent.setArgument("ID",Variant(player->getID()));

	EventManager::getInstance()->addEventToQueue(shotHitEvent);
	EventManager::getInstance()->addEventToQueue(playerHitEvent);

	return;
}

void collision_handler_enemyshot__barricade(GameObject *go1,GameObject *go2)
{
	Shot shot = NULL;
	Barricade barricade = NULL;

	if(go1->getType() == "Barricade"){
		barricade = go1;
		shot = go2;
	}else{
		shot = go1;
		barricade = go2;
	}

	Event shotHitEvent(SI_Events::ShotHit);
	Event barricadeHitEvent(SI_Events::BarricadeHit);
	Event damageTakenEvent(SI_Events::DamageTaken);

	damageTakenEvent.setArgument("ID",Variant(barricade->getID()));
	shotHitEvent.setArgument("ID",Variant(shot->getID()));
	barricadeHitEvent.setArgument("ID",Variant(barricade->getID()));

	//Tell the barricade that is has been hit.
	barricade->broadcastMessage(damageTakenEvent);

	//Spread the other events to whoever listens to it.
	EventManager::getInstance()->addEventToQueue(shotHitEvent);
	EventManager::getInstance()->addEventToQueue(barricadeHitEvent);

	return;
}

void collision_handler_powerup__player(GameObject *go1,GameObject *go2)
{
	Player player = NULL;
	PowerUp powerup = NULL;

	if(go1->getType() == "Player"){
		player = go1;
		powerup = go2;
	}else{
		powerup = go1;
		player = go2;
	}

	Event powerUpHit(SI_Events::PowerUpCollected);

	powerUpHit.setArgument("ID",Variant(powerup->getID()));
	powerUpHit.setArgument("Player_ID",Variant(player->getID()));

	EventManager::getInstance()->addEventToQueue(powerUpHit);
}
////////////////////


SpaceInvadersState::SpaceInvadersState()
{
	ARR_SET_TO_NULL(m_objects,CONST_AMOUNT_OBJECTS);
}

SpaceInvadersState::~SpaceInvadersState()
{
	ARR_DELETE_ONE_BY_ONE(m_objects,CONST_AMOUNT_OBJECTS);
}

bool SpaceInvadersState::start()
{
	//add all the listener.
	ADD_LISTENER(this,Events::KeyboardPressed);
	ADD_LISTENER(this,SI_Events::BarricadeHit);
	ADD_LISTENER(this,SI_Events::EnemyHit);
	ADD_LISTENER(this,SI_Events::PlayerHit);
	ADD_LISTENER(this,SI_Events::PowerUpCollected);
	ADD_LISTENER(this,SI_Events::ShotCreated);

	//Subscribe for collisions
	CollisionManager::subscribeCollisionHandler("Player","Powerup",collision_handler_powerup__player);
	CollisionManager::subscribeCollisionHandler("Player","EnemyShot",collision_handler_enemyshot__player);
	CollisionManager::subscribeCollisionHandler("Enemy","PlayerShot",collision_handler_enemy__playershot);
	CollisionManager::subscribeCollisionHandler("Barricade","EnemyShot",collision_handler_enemyshot__barricade);

	//start a new round.
	_restartRound();

	//spawn players afterwards.
	_spawnPlayers();

	//Spawn barriers
	_spawnBarriers();

	return true;
}

void SpaceInvadersState::update( U32 deltaTime )
{
	//update every gameobject
	for(int i = 0;i < CONST_AMOUNT_OBJECTS;++i){
		if(m_objects[i] != NULL){
			m_objects[i]->update(deltaTime);
		}
	}
}

void SpaceInvadersState::end()
{
	//remove all the listener;
	REMOVE_LISTENER(this,Events::KeyboardPressed);
	REMOVE_LISTENER(this,SI_Events::BarricadeHit);
	REMOVE_LISTENER(this,SI_Events::EnemyHit);
	REMOVE_LISTENER(this,SI_Events::PlayerHit);
	REMOVE_LISTENER(this,SI_Events::PowerUpCollected);
	REMOVE_LISTENER(this,SI_Events::ShotCreated);

	//delete all objects.
	_deleteObjects();
}

bool SpaceInvadersState::handleEvent( Event const &gameEvent )
{
	if(gameEvent.getType() == Events::KeyboardPressed){
		U16 key = gameEvent.getArgument("key").toInteger;
		if(key == K15_ESC){
			//Show Pause Menu
			myGame->getStateMachine()->setNextState(new MenuState());
			return true;

			//JUST FOR TESTING
		}else if(key == K15_LEFT){
			Event newEvent(SI_Events::ChangeHorizontalDirection);
			EventManager::getInstance()->addEventToQueue(newEvent);
		}
	}else if(gameEvent.getType() == SI_Events::ShotCreated){
		U32 gameObjectID = gameEvent.getArgument("ID").toInteger;
		GameObject *object = _getObject(gameObjectID);
		if(object != NULL){
			if(object->getType() == "Player"){
				_spawnShot(object->getPosition(),0,5,PLAYERSHOT_FLAG);
			}else if(object->getType() == "Enemy"){
				_spawnShot(object->getPosition(),0,-5,ENEMYSHOT_FLAG);
			}
		}
	}
	
	return false;
}

void SpaceInvadersState::_restartRound()
{
	//spawn new enemies
	_spawnEnemies();
}

void SpaceInvadersState::_deleteObjects()
{
	//Delete each and every gameobject
	for(int i = 0;i < CONST_AMOUNT_OBJECTS;++i){
		SAFE_DELETE(m_objects[i]);
	}
}

//Spawn Functions
void SpaceInvadersState::_spawnEnemies()
{
	S16 x = 150;
	S16 y = 0;

	for(int i  = CONST_ENEMY_OFFSET;i < CONST_AMOUNT_ENEMIES;++i){
		m_objects[i] = GameObjectFactory::createEnemy("Enemy",i);
		m_objects[i]->setPosition(x,y);

		x += 50;
		if(x > 650){
			x = 150;
			y += 50;
		}
	}
}

void SpaceInvadersState::_spawnBarriers()
{
	S16 x = 100;
	S16 y = 400;

	for(int i = CONST_BARRICADE_OFFSET;i < CONST_AMOUNT_MAX_BARRICADES + CONST_BARRICADE_OFFSET;++i){
		m_objects[i]= GameObjectFactory::createBarrier();
		//m_barriers[i].m_health = 10; <-- Move to a Component;
		m_objects[i]->setPosition(x,y);
		x += 300;
	}
}

void SpaceInvadersState::_spawnPlayers()
{
	S16 x = 200;
	S16 y = 550;

	for(int i = CONST_PLAYER_OFFSET;i < CONST_AMOUNT_MAX_PLAYERS + CONST_PLAYER_OFFSET;++i){
		m_objects[i]= GameObjectFactory::createPlayer(i - (CONST_PLAYER_OFFSET - 1));
		m_objects[i]->setPosition(x,y);
		x += 300;
	}
}

void SpaceInvadersState::_spawnShot(const Position& position,int velX,int velY,int shotFlags)
{
	for(int i = CONST_SHOT_OFFSET;i < CONST_AMOUNT_MAX_SHOTS;++i){
		if(m_objects[i] == NULL){
			if((shotFlags & PLAYERSHOT_FLAG) == PLAYERSHOT_FLAG){
				if((shotFlags & POWERSHOT_FLAG) == POWERSHOT_FLAG){
					m_objects[i] = GameObjectFactory::createShot("PlayerShot",velX,velY);
				}else{
					m_objects[i] = GameObjectFactory::createShot("PlayerShot",velX,velY);
				}
			}else if((shotFlags & ENEMYSHOT_FLAG) == ENEMYSHOT_FLAG){
				m_objects[i] = GameObjectFactory::createShot("EnemyShot",velX,velY);
			}
		}
	}
}

GameObject* SpaceInvadersState::_getObject( int gID )
{
	for(int i = 0;i < CONST_AMOUNT_OBJECTS;++i){
		if(m_objects[i] != NULL){
			if(m_objects[i]->getID() == gID){
				return m_objects[i];
			}
		}
	}

	return NULL;
}
