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

#include "MainMenuState.h"

#include "K15_Engine/IGraphic.h"
#include "K15_Engine/Game.h"
#include "K15_Engine/GameObject.h"
#include "K15_Engine/EventManager.h"
#include "K15_Engine/Events.h"
#include "K15_Engine/Keymap.h"

using namespace K15_SpaceInvaders;

AbstractMenuState::AbstractMenuState()
{
	m_background = myGame->getGraphicManager()->createGraphic("background.png");

	m_menu_background = myGame->getGraphicManager()->createGraphic("menu_background.png");
	m_menu_background->setColorkey(255,0,255);
	m_margin = 0;
	m_itemMargin = 0;
}

bool AbstractMenuState::start()
{
	ADD_LISTENER(this,Events ::MouseMoved);
	ADD_LISTENER(this,Events::KeyboardPressed);
	ADD_LISTENER(this,Events::MousePressed);

	_alignItems();
	_alignBackground();

	m_selectedItem = m_itemList.begin();
	
	return true;
}

void AbstractMenuState::update(U32 deltaTime)
{
	if(m_background != NULL){
		DRAW_GRAPHIC(m_background);
	}

	if(m_menu_background != NULL){
		DRAW_GRAPHIC(m_menu_background);
	}

	for(MenuIterator iter = m_itemList.begin();iter != m_itemList.end();iter++){
		MenuItem currentItem = (*iter);
		currentItem->update(deltaTime);
	}
}

void AbstractMenuState::end()
{
	MenuIterator menuIter_Start = m_itemList.begin();
	MenuIterator menuIter_End = m_itemList.end();

	while(menuIter_Start != menuIter_End){
		SAFE_DELETE((*menuIter_Start));
		menuIter_Start++;
	}

	REMOVE_LISTENER(this,Events::MouseMoved);
	REMOVE_LISTENER(this,Events::KeyboardPressed);
	REMOVE_LISTENER(this,Events::MousePressed);
}

bool AbstractMenuState::handleEvent(Event const &gameEvent)
{
	MenuIterator oldSelectedItem = m_selectedItem;

	if(gameEvent.getType() == Events::MouseMoved){
		//Mouse Moved
		int mouse_x = gameEvent.getArgument("x").toInteger;
		int mouse_y = gameEvent.getArgument("y").toInteger;

		//Check if the mouse intersects with one of the objects.
		for(MenuIterator iter = m_itemList.begin();iter != m_itemList.end();++iter){
			MenuItem currentItem = (*iter);

			int object_x = currentItem->getPosition().getX();
			int object_y = currentItem->getPosition().getY();
			int object_w = currentItem->getSize().getWidth();
			int object_h = currentItem->getSize().getHeight();

			if(mouse_x < (object_x + object_w) && mouse_x > object_x){
				if(mouse_y < (object_y + object_h) && mouse_y > object_y){
					//Found intersection.
					m_selectedItem = iter;
				}
			}
		}
	}else if(gameEvent.getType() == Events::KeyboardPressed || gameEvent.getType() == Events::MousePressed){
		if(gameEvent.getType() == Events::KeyboardPressed){

			//Keyboard
			int key = gameEvent.getArgument("key").toInteger;

			if(key == K15_ENTER){
				//Send message 
				(*m_selectedItem)->broadcastMessage(SI_Events::ActivateItem);
			}else if(key == K15_UP){
				if(m_selectedItem != m_itemList.begin()){
					--m_selectedItem;
				}
			}else if(key == K15_DOWN){
				if(m_selectedItem != m_itemList.end()){
					++m_selectedItem;

					if(m_selectedItem == m_itemList.end()){
						--m_selectedItem;
					}
				}
			}

		}else{
			//Mouse

			//Send message
			(*m_selectedItem)->broadcastMessage(SI_Events::ActivateItem);
		}
	}

	if(oldSelectedItem != m_selectedItem){
		Event gainedRotateEvent(SI_Events::RotateObject);
		gainedRotateEvent.setArgument("angle",Variant(-10.0f));

		Event lostRotateEvent(SI_Events::RotateObject);
		lostRotateEvent.setArgument("angle",Variant(0.0f));

		(*oldSelectedItem)->broadcastMessage(Event(SI_Events::ItemFocusLost));
		(*oldSelectedItem)->broadcastMessage(lostRotateEvent);

		(*m_selectedItem)->broadcastMessage(Event(SI_Events::ItemFocusGained));
		(*m_selectedItem)->broadcastMessage(gainedRotateEvent);
	}

	return true;
}

void AbstractMenuState::_alignItems()
{
	const Position upperLeftMenuPos = Position(300,250);//Position(m_margin + m_menuCenter.getX() - m_menuSize.getWidth()/2,m_margin + m_menuCenter.getY() - m_menuSize.getHeight()/2);

	Position curPos = upperLeftMenuPos;

	for(MenuItemList::iterator iter = m_itemList.begin();iter != m_itemList.end();iter++){
		MenuItem curItem = (*iter);
		const Size& curItemSize = curItem->getSize();
		
		curItem->setPosition(curPos);

		curPos = curPos + Position(0,curItemSize.getHeight() + m_itemMargin);
	}
}

void AbstractMenuState::_alignBackground()
{
	if(m_menu_background != NULL){
		const Position upperLeftMenuPos = Position(220,180);//Position(m_margin + m_menuCenter.getX() - m_menuSize.getWidth()/2,m_margin + m_menuCenter.getY() - m_menuSize.getHeight()/2);
		m_menu_background->setPosition(upperLeftMenuPos);
		//m_menu_background->setSize(m_menuSize);
	}
	
	
}
