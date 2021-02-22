#include "PlayState.h"

namespace Connect
{
	void PlayState::PlayGameButtonFunction()
	{
		LOG_TRACE("Play Game button pressed");
	}

	void PlayState::AIButtonFunction()
	{
		LOG_TRACE("AI choice button pressed");
		if (recentButton->id == 1)
		{
			m_IsPlayer1AI = !m_IsPlayer1AI;
			if (m_IsPlayer1AI)
				m_IsPlayer1AIButton.SetText("AI: yes", true);
			else
				m_IsPlayer1AIButton.SetText("AI: no", true);
		}
		else
		{
			m_IsPlayer2AI = !m_IsPlayer2AI;
			if (m_IsPlayer2AI)
				m_IsPlayer2AIButton.SetText("AI: yes", true);
			else
				m_IsPlayer2AIButton.SetText("AI: no", true);
		}
	}
}