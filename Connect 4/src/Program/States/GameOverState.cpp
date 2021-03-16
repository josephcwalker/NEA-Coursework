#include "GameOverState.h"

#include "Program/Program.h"

namespace Connect
{
	void GameOverState::TrainAI()
	{
		// Train human player's AI on this game and also any saved games
		bool isPlayer1AI = dynamic_cast<Connect4AI*>(m_Player1);
		bool isPlayer2AI = dynamic_cast<Connect4AI*>(m_Player2);

		// Save this game to a file
		m_Player1->SaveGame(m_PreviousGame, true);
		m_Player2->SaveGame(m_PreviousGame, false);

		// These may take a while so multi-threading may need to be used
		m_Player1->TrainOnGames();
		m_Player2->TrainOnGames();
	}

	void GameOverState::BackButtonFunction()
	{
		// Free memory holding accounts
		delete m_Player1;
		delete m_Player2;

		Program::s_Instance->PopState();
	}

	void GameOverState::RematchButtonFunction()
	{
		// Create Connect Game State with same players again
		bool isPlayer1AI = dynamic_cast<Connect4AI*>(m_Player1);
		bool isPlayer2AI = dynamic_cast<Connect4AI*>(m_Player2);

		m_Player1->LoadNeuralNetwork();
		m_Player2->LoadNeuralNetwork();

		Program::s_Instance->PushState(new ConnectGameState(m_Player1, isPlayer1AI, m_Player2, isPlayer2AI));
		Program::s_Instance->PopState();
	}
}