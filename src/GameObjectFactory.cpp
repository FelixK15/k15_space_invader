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

#include "GameObjectFactory.h"
#include "RenderableComponent.h"
#include "UsefulFunctions.h"

#include "SpaceInvadersState.h"
#include "ControlComponentPlayer1.h"
#include "ControlComponentPlayer2.h"
#include "MoveableComponent.h"
#include "HighscoreState.h"
#include "HighscoreComponent.h"

#include "K15_Engine/IGraphic.h"
#include "K15_Engine/EventManager.h"
#include "K15_Engine/AnimatedComponent.h"
#include "K15_Engine/VersatileComponent.h"
#include "K15_Engine/EventListenerComponent.h"
#include "K15_Engine/CollisionComponent.h"
#include "K15_Engine/Game.h"

using namespace K15_SpaceInvaders;

GameObject* GameObjectFactory::createMenuItem( const String &type )
{
	GameObject *newObject = new GameObject(type);
	newObject->setSize(120,32);

	AbstractComponent *activatedComponent = NULL;
	GraphicPtr graphic;

	if(type == "Start"){
		activatedComponent = new VersatileComponent("Start_Activator",[](const Event& gameEvent,GameObject* object)->bool{
			if(gameEvent.getType() == SI_Events::ActivateItem){
				myGame->getStateMachine()->setNextState(new SpaceInvadersState());
				myGame->getStateMachine()->removeCurState();
				return true;
			}

			return false;
		},[](U32 deltaTime,GameObject* object){

		});
		
		graphic = myGame->getGraphicManager()->createGraphic("start.png");
	}else if(type == "Start 2Player"){
		activatedComponent = new VersatileComponent("Start_2P_Activator",[](const Event& gameEvent,GameObject* object)->bool{
			if(gameEvent.getType() == SI_Events::ActivateItem){
				myGame->getStateMachine()->setNextState(new SpaceInvadersState(true));
				myGame->getStateMachine()->removeCurState();
				return true;
			}

			return false;
		},[](U32 deltaTime,GameObject* object){

		});

		graphic = myGame->getGraphicManager()->createGraphic("start2P.png");
	}else if(type == "Show Highscore"){
		activatedComponent = new VersatileComponent("Highscore_Activator",[](const Event& gameEvent,GameObject* object)->bool{
			if(gameEvent.getType() == SI_Events::ActivateItem){
				myGame->getStateMachine()->setNextState(new HighscoreState());
				myGame->getStateMachine()->removeCurState();
				return true;
			}

			return false;
		},[](U32 deltaTime,GameObject* object){

		});
		graphic = myGame->getGraphicManager()->createGraphic("highscore.png");
	}else if(type == "Quit"){
		graphic = myGame->getGraphicManager()->createGraphic("quit.png");
		activatedComponent = new VersatileComponent("Quit_Activator",[](const Event& gameEvent,GameObject* object)->bool{
			if(gameEvent.getType() == SI_Events::ActivateItem){
				Event newEvent(Events::WindowClosed);
				EventManager::getInstance()->triggerEvent(newEvent);
				return true;
			}

			return false;
		},[](U32 deltaTime,GameObject* object){

		});
	}

	graphic->setColorkey(255,0,255);
	graphic->setGraphicAsOrigin();
	RenderableComponent *renderComponent = new RenderableComponent(graphic);

	newObject->addComponent(activatedComponent);
	newObject->addComponent(renderComponent);

	return newObject;
}

GameObject* GameObjectFactory::createEnemy( const String &type,int factor )
{
	//GameObject Creation//
	GameObject *newObject = new GameObject(type);
	newObject->setSize(32,32);

	//Graphic Creation//
	GraphicPtr rootGraphic = NULL; 

	if(type == "Alien"){
		rootGraphic = myGame->getGraphicManager()->createGraphic("enemy4.png");
		rootGraphic->setColorkey(255,0,255);
		rootGraphic->fillWithColor(120,0,0,80);
	}else if(factor / 11 == 1){
		rootGraphic = myGame->getGraphicManager()->createGraphic("enemy1.png");
		rootGraphic->setColorkey(255,0,255);
		rootGraphic->fillWithColor(120,0,0,80);
	}else if(factor / 11 == 2){
		rootGraphic = myGame->getGraphicManager()->createGraphic("enemy2.png");
		rootGraphic->setColorkey(255,0,255);
		rootGraphic->fillWithColor(0,80,80,80);
	}else if(factor / 11 == 3){
		rootGraphic = myGame->getGraphicManager()->createGraphic("enemy3.png");
		rootGraphic->setColorkey(255,0,255);
		rootGraphic->fillWithColor(0,200,150,80);
	}else{
		rootGraphic = myGame->getGraphicManager()->createGraphic("enemy1.png");
		rootGraphic->setColorkey(255,0,255);
		rootGraphic->fillWithColor(80,80,0,80);
	}

	rootGraphic->setGraphicAsOrigin();

	GraphicPtr normalAnimation_1 = rootGraphic->cutRect(0,0,32,32);
	GraphicPtr normalAnimation_2 = rootGraphic->cutRect(32,0,32,32);
	GraphicPtr normalAnimation_3 = rootGraphic->cutRect(64,0,32,32);

	//GraphicPtr attackAnimation_1 = rootGraphic->cutRect(96,0,32,32);

	AnimatedComponent *animComponent = new AnimatedComponent();

	Animation idleAnimation("Idle");

	if(factor % 2 == 0){
		idleAnimation.addAnimationData(AnimationData(normalAnimation_1,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_2,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_3,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_2,200));
	}else{
		idleAnimation.addAnimationData(AnimationData(normalAnimation_2,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_1,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_2,200));
		idleAnimation.addAnimationData(AnimationData(normalAnimation_3,200));
	}


	animComponent->addAnimation(idleAnimation);
	
	animComponent->startAnimation("Idle");
	newObject->addComponent(animComponent);

	return newObject;
}

GameObject* GameObjectFactory::createPlayer(const String &type)
{
	//GameObject Creation//
	GameObject *newObject = new GameObject(type);
	newObject->setSize(32,32);

	//Graphic Creation//
	GraphicPtr graphic = myGame->getGraphicManager()->createGraphic("player.png");
	graphic->setColorkey(255,0,255);
	if(type == "Player2"){
		graphic->fillWithColor(180,0,80,80);
	}

	graphic->setGraphicAsOrigin();
	
	//Component Creation//
	AnimatedComponent *anim_component = new AnimatedComponent();

	Animation anim("idle");
	anim.addAnimationData(AnimationData(graphic->cutRect(0,0,32,32),200));
	anim.addAnimationData(AnimationData(graphic->cutRect(32,0,32,32),200));
	anim.addAnimationData(AnimationData(graphic->cutRect(64,0,32,32),200));
	anim.addAnimationData(AnimationData(graphic->cutRect(96,0,32,32),200));

	anim_component->addAnimation(anim);
	anim_component->startAnimation("idle");

	EventListenerComponent *keypress_eventListener = new EventListenerComponent(Events::KeyboardPressed);
	EventListenerComponent *keyrelease_eventListener = new EventListenerComponent(Events::KeyboardReleased);
	//HighscoreComponent *highscoreComponent = new HighscoreComponent();

	AbstractComponent *controlComp = NULL;
	if(type == "Player"){
		controlComp = new ControlComponentPlayer1();
	}else{
		controlComp = new ControlComponentPlayer2();
	}
	

	//Components get added to the Game Object//
	newObject->addComponent(anim_component);
	newObject->addComponent(keypress_eventListener);
	newObject->addComponent(keyrelease_eventListener);
	newObject->addComponent(controlComp);
	//newObject->addComponent(highscoreComponent);

	return newObject;
}

GameObject* GameObjectFactory::createShot(const String& identifier,int velX,int velY)
{
	//GameObject Creation//
	GameObject *newObject = new GameObject(identifier);
	newObject->setSize(12,12);

	//Graphic Creation//
	GraphicPtr bulletgraphic = myGame->getGraphicManager()->createGraphic("bullet.png");
	bulletgraphic->setColorkey(255,0,255);

	//Component Creation//
	AbstractComponent *moveComponent = new MoveableComponent(velX,velY);
	RenderableComponent *rendercomponent = new RenderableComponent(bulletgraphic);

	//Components get added to the Game Object//
	newObject->addComponent(moveComponent);
	newObject->addComponent(rendercomponent);

	return newObject;
}

GameObject* GameObjectFactory::createExplosion(int x,int y)
{
	//Game Object Creation//
	GameObject *newObject = new GameObject("Explosion");
	newObject->setSize(32,32);
	newObject->setPosition(x,y);

	//Graphic Creation//
	GraphicPtr explosionGraphic = myGame->getGraphicManager()->createGraphic("explosion.png");
	explosionGraphic->setColorkey(255,0,255);
	explosionGraphic->fillWithColor(120,50,0,70);
	explosionGraphic->setGraphicAsOrigin();

	//Animation Creation//
	AnimatedComponent *animComponent = new AnimatedComponent();
	Animation animation("Explosion");
	animation.addAnimationData(AnimationData(explosionGraphic->cutRect(0,0,32,32),200));
	animation.addAnimationData(AnimationData(explosionGraphic->cutRect(32,0,32,32),80));
	animation.addAnimationData(AnimationData(explosionGraphic->cutRect(64,0,32,32),80));
	animation.addAnimationData(AnimationData(explosionGraphic->cutRect(96,0,32,32),80));
	animComponent->addAnimation(animation);
	animComponent->startAnimation("Explosion");
	
	//Components get added to the Game Object//
	newObject->addComponent(animComponent);

	return newObject;
}

GameObject* GameObjectFactory::createBarrier()
{
	//Game Object Creation//
	GameObject *newObject = new GameObject();
	newObject->setSize(64,64);

	//Graphic Creation//
	GraphicPtr barrierGraphic = myGame->getGraphicManager()->createGraphic("barrier.png");
	barrierGraphic->setColorkey(255,0,255);
	barrierGraphic->setGraphicAsOrigin();

	//Animation Creation//
	AnimatedComponent *animcomponent = new AnimatedComponent();

	//Animation "normal"//
	Animation normal("normal");
	normal.addAnimationData(AnimationData(barrierGraphic->cutRect(0,0,64,64),10));

	//Animation "hurt"//
	Animation hurt("hurt");
	hurt.addAnimationData(AnimationData(barrierGraphic->cutRect(64,0,64,64),10));

	//Animation "badly_hurt"//
	Animation badly_hurt("badly_hurt");
	badly_hurt.addAnimationData(AnimationData(barrierGraphic->cutRect(128,0,64,64),10));

	//Animation "dead"//
	Animation dead("dead");
	dead.addAnimationData(AnimationData(barrierGraphic->cutRect(192,0,63,64),10));

	//Animations get added to the AnimationComponent//
	animcomponent->addAnimation(normal);
	animcomponent->addAnimation(hurt);
	animcomponent->addAnimation(badly_hurt);
	animcomponent->addAnimation(dead);

	//Animation "normal" gets started.
	animcomponent->startAnimation("normal");

	//Components get added to the Game Object//
	newObject->addComponent(animcomponent);

	return newObject;
}

GameObject* GameObjectFactory::createPowerUp( int x,int y,int *kindOfPowerUp )
{
	const int FASTER_PLAYER = 1;
	const int ONE_UP = 2;
	const int REPAIR_BARRIER = 3;
	const int POWER_SHOT = 4;

	//Game Object Creation//
	GameObject *newObject = new GameObject();
	newObject->setSize(30,20);

	GraphicPtr graphic = myGame->getGraphicManager()->createGraphic("powerup.png");
	graphic->setColorkey(255,0,255);

	//Determine the type of the powerup
	int random = (rand() % 4);

	if(random == FASTER_PLAYER){
		graphic->fillWithColor(0,180,0,80);
	}else if(random == ONE_UP){
		graphic->fillWithColor(0,0,180,80);
	}else if(random == REPAIR_BARRIER){
		graphic->fillWithColor(0,180,180,80);
	}else if(random == POWER_SHOT){
		graphic->fillWithColor(180,0,180,80);
	}

	*kindOfPowerUp = random;

	graphic->setGraphicAsOrigin();

	Animation idle("idle");
	idle.addAnimationData(AnimationData(graphic->cutRect(0,0,30,20),200));
	idle.addAnimationData(AnimationData(graphic->cutRect(30,0,30,20),200));
	idle.addAnimationData(AnimationData(graphic->cutRect(60,0,30,20),200));

	AnimatedComponent *animcomponent = new AnimatedComponent();
	animcomponent->addAnimation(idle);
	animcomponent->startAnimation("idle");

	newObject->addComponent(animcomponent);
	newObject->setPosition(x,y);

	return newObject;
}

GameObject* GameObjectFactory::createPoints( const Position &pos,int points )
{
	GameObject *newObject = new GameObject("Points");

	RenderableComponent *rendercomponent = NULL;
	GraphicPtr graphic = myGame->getGraphicManager()->createGraphic("points.png");
	graphic->setColorkey(255,0,255);

	if(points == 100){
		rendercomponent = new RenderableComponent(graphic->cutRect(0,0,32,32));
	}else{
		rendercomponent = new RenderableComponent(graphic->cutRect(32,0,32,32));
	}

	newObject->addComponent(rendercomponent);
	newObject->setPosition(pos);

	return newObject;
}
