#ifndef __SI_HIGHSCORE_ENTER_STATE__
#define __SI_HIGHSCORE_ENTER_STATE__

#include "K15_Engine/IState.h"
#include "K15_Engine/DrawableString.h"
#include "K15_Engine/IEventListener.h"

using namespace K15_Engine;

namespace K15_SpaceInvaders
{
	class HighscoreEnterState : public IState, public IEventListener
	{
	public:
		HighscoreEnterState(bool twoPlayers,int ptP1,int ptP2);

		virtual bool start() override;

		virtual void update(U32 deltaTime) override;
	
		virtual bool handleEvent(Event const &gameEvent) override;

		virtual void end() override;

	private:

		void _saveHighscore();

		GraphicPtr m_background;

		DrawableString m_namePlayer1;
		DrawableString m_namePlayer2;
		DrawableString m_highScorePlayer1;
		DrawableString m_highScorePlayer2;

		int m_player1_points;
		int m_player2_points;
		
		bool m_twoPlayers;

		bool m_player1_ready;
		bool m_player2_ready;
	};
}

#endif //__SI_HIGHSCORE_ENTER_STATE__