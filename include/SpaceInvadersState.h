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

#ifndef __SI_ABSTRACT_SPACE_INVADERS_STATE__
#define __SI_ABSTRACT_SPACE_INVADERS_STATE__

#include "StdInclude.h"
#include "Difficulty.h"

#include "K15_Engine/DrawableString.h"
#include "K15_Engine/IState.h"
#include "K15_Engine/IEventListener.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	typedef GameObject* Enemy;
	typedef GameObject* Player;
	typedef GameObject* Shot;
	typedef GameObject* Explosion;
	typedef GameObject* Barrier;
	typedef GameObject* PowerUp;
	typedef GameObject* Points;

	enum ENUM_ENEMY_DIRECTION{
		DIRECTION_RIGHT,
		DIRECTION_LEFT,
		DIRECTION_DOWN
	};

	class SpaceInvadersState : public IState, public IEventListener
	{

		struct ExplosionWrapper{
			ExplosionWrapper(){ m_explosion = NULL; m_lifespan = 0;}

			Explosion m_explosion;
			U32 m_lifespan;
		};

		struct BarrierWrapper{
			BarrierWrapper(){m_barrier = NULL; m_health = 0;}

			Barrier m_barrier;
			S32 m_health;
		};

		struct PointsWrapper{
			PointsWrapper(){m_points = NULL;m_lifespan = 0;}

			Points m_points;
			U32 m_lifespan;
		};

	public:
		SpaceInvadersState(bool twoPlayer = false){m_twoplayer = twoPlayer;}

		virtual bool start() override;

		virtual void update(U32 deltaTime) override;

		virtual void end() override;

		virtual bool handleEvent(const Event& gameEvent) override;

		Shot getShot(){return m_shot;}

		void setShot(Shot shot,int player);

	protected:

		void _createEnemies();

		void _createBarriers();

		void _createExplosion(const Position& pos);

		void _updatePoints(U32 deltaTime);

		void _updateExplosions(U32 deltaTime);

		void _updateEnemies(U32 deltaTime);

		void _updateShots(U32 deltaTime);

		void _updateBarriers(U32 deltaTime);

		void _updatePlayerShot(U32 deltaTime);

		void _updatePowerUp(U32 deltaTime);

		void _spawnEnemyShot(U32 deltaTime);

		void _updatePlayerInfos();

		void _spawnPowerUp(const Position& pos);

		void _spawnUpperAlien();

		void _startNewRound();

		void _refreshBarriers();

		void _showPoints(const Position& pos,int points);

	private:
		int m_curPowerup;

		bool m_twoplayer;

		static bool POWERUP_ACTIVE;

		static U32 DYNAMIC_TIMER_POWERUP_FASTER_SHOT;

		static const U32 CONST_TIMER_POWERUP_FASTER_SHOT;

		static const F32 SHOT_MOVEFACTOR;

		static const U32 CONST_AMOUNT_ENEMIES = 44;

		static const U32 AMOUNT_BARRIERS = 3;

		static const U32 MAX_EXPLOSIONS = 10;

		static const U32 MAX_SHOTS = 20;

		static const Difficulty GODLIKE_DIFFICULTY;

		static const Difficulty HARD_DIFFICULTY;

		static const Difficulty MEDIUM_DIFFICULTY;

		static const Difficulty EASY_DIFFICULTY;

		static U32 DYNAMIC_AMOUNT_EMENIES;

		static Difficulty m_curDifficulty;

		static U32 MILLISECONDS_BETWEEN_MOVEMENT;

		static const F32 CONST_MOVE_FACTOR;

		static F32 DYNAMIC_MOVE_FACTOR;

		static bool PLAYER_ONE_DEAD;

		static bool PLAYER_TWO_DEAD;

		static const U32 CONST_SPAWN_TIME_PLAYER_1 = 2000;
		static U32 DYNAMIC_SPAWN_TIME_PLAYER1;

		static const U32 CONST_SPAWN_TIME_PLAYER_2 = 2000;
		static U32 DYNAMIC_SPAWN_TIME_PLAYER2;

		static U32 TEMP_MILLISECONDS_BETWEEN_MOVEMENT;

		static U32 TEMP_MILLISECONDS_BETWEEN_SHOTS;

		static U32 DYNAMIC_UPPER_ALIEN_SPAWNTIME;
		static const U32 STATIC_UPPER_ALIEN_SPAWNTIME = 10000;

		static bool ENEMY_TOUCHED_BORDER;

		static ENUM_ENEMY_DIRECTION ENEMY_DIRECTION;

		DrawableString m_player1_info;

		DrawableString m_player2_info;

		int m_lives_p1;

		int m_lives_p2;

		int m_highscore_p1;

		int m_highscore_p2;



		GraphicPtr m_background;

		Enemy m_upperAlien;

		Enemy m_enemies[CONST_AMOUNT_ENEMIES];

		Shot m_enemyshots[MAX_SHOTS];

		BarrierWrapper m_barriers[AMOUNT_BARRIERS];

		ExplosionWrapper m_explosions[MAX_EXPLOSIONS];

		PointsWrapper m_points[MAX_SHOTS];

		PowerUp m_powerUp;

		Player m_player;

		Player m_player2;

		Shot m_shot;

		Shot m_shot2;
	};
}

#endif //__SI_ABSTRACT_SPACE_INVADERS_STATE__
