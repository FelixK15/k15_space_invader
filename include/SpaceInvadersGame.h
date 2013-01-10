#ifndef __SI_GAME__
#define __SI_GAME__

#include "K15_Engine\Game.h"
#include "K15_Engine\IEventListener.h"

#include "StdInclude.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{

	class SpaceInvadersGame : public Game, public IEventListener
	{
	public:

		virtual void start() override;

		virtual bool handleEvent(const Event& gameEvent) override;

		virtual void stop() override;
	};
}

#endif //__SI_GAME__q