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

#ifndef __SI_RENDERABLE_COMPONENT__
#define __SI_RENDERABLE_COMPONENT__

#include "K15_Engine/AbstractComponent.h"

#include "StdInclude.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class RenderableComponent : public AbstractComponent
	{
	public:

		RenderableComponent(const String& rsgraphic);

		RenderableComponent(GraphicPtr graphic);

		void setGraphic(GraphicPtr graphic){m_graphic = graphic;}

		void setGraphic(const String& graphicResource);

		virtual AbstractComponent *clone() override;

		virtual bool handleMessage(const Event &gameEvent ) override;

		virtual void setSize(const Size& size) override;

		virtual void setPosition(const Position& position) override;

		virtual void setAngle(S16 angle) override;

		virtual void update(U32 gameTime) override;

	private:
		
		GraphicPtr m_graphic;

	};
}

#endif //__SI_RENDERABLE_COMPONENT__
