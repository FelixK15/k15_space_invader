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

#ifndef __SI_ABSTRACTMENU_STATE__
#define __SI_ABSTRACTMENU_STATE__

#include "K15_Engine/IEventListener.h"
#include "K15_Engine/IState.h"
#include "StdInclude.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	typedef int ItemMargin;
	typedef int MenuMargin;
	typedef GameObject* MenuItem;
	typedef Size MenuSize;
	typedef Position MenuCenter;
	typedef std::list<MenuItem> MenuItemList;
	typedef MenuItemList::iterator MenuIterator;

	class AbstractMenuState : public IState, public IEventListener
	{
	public:

		virtual bool start() override;

		virtual void update(U32 deltaTime) override;

		virtual void end() override;

		virtual bool handleEvent(Event const &gameEvent) override;

	protected:
		AbstractMenuState();
		void _addMenuItem(MenuItem item){m_itemList.push_back(item);}
		void _setMenuSize(const MenuSize &size){m_menuSize = size;}
		void _setMenuCenterPos(const MenuCenter &pos){m_menuCenter = pos;}
		void _setBackground(GraphicPtr graphic){m_background = graphic;}
		void _setMargin(U32 margin){m_margin = margin;}
		void _setMarginBetweenItems(U32 margin){m_itemMargin = margin;}

	private:
		
		void _alignItems();
		void _alignBackground();

	private:
		MenuSize m_menuSize;
		MenuCenter m_menuCenter;
		MenuMargin m_margin;
		ItemMargin m_itemMargin;

		MenuItemList m_itemList;
		MenuIterator m_selectedItem;
		GraphicPtr m_menu_background;
		GraphicPtr m_background;
	};
}

#endif //__SI_ABSTRACTMENU_STATE__
