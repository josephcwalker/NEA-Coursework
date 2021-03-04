#include "PlayState.h"

#include "Program/Account.h"
#include "Program/Connect4Account.h"
#include "Program/Connect4AI.h"
#include "Program/Program.h"

#include "ConnectGameState.h"

#include <filesystem>

namespace Connect
{
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

	void PlayState::PlayGameButtonFunction()
	{
		LOG_TRACE("Play Game button pressed");

		// Reset error messages
		m_Player1ErrorMessages.SetText("");
		m_Player2ErrorMessages.SetText("");

		// Check inputs are valid for both players
		bool player1Valid = true;
		bool player2Valid = true;

		std::string error_message = CheckInputsAreValid(m_Player1Username, m_Player1Password, m_IsPlayer1AI);

		if (!error_message.empty())
		{
			player1Valid = false;
			m_Player1ErrorMessages.SetText("Error: " + error_message);
			LOG_WARN("Error selecting player 1");
		}

		error_message = CheckInputsAreValid(m_Player2Username, m_Player2Password, m_IsPlayer2AI);

		if (!error_message.empty())
		{
			player2Valid = false;
			m_Player2ErrorMessages.SetText("Error: " + error_message);
			LOG_WARN("Error selecting player 2");
		}


		// Check both players are valid
		if (player1Valid && player2Valid)
		{
			LOG_TRACE("Both Players are valid");

			// Create either AI or Human players for the game
			Connect4Account *player1 = m_IsPlayer1AI ? new Connect4AI(m_Player1Username.GetInput())
													 : new Connect4Account(m_Player1Username.GetInput());
			Connect4Account *player2 = m_IsPlayer2AI ? new Connect4AI(m_Player1Username.GetInput())
													 : new Connect4Account(m_Player1Username.GetInput());

			// Replace this state with Game state
			Program::s_Instance->PopState();
			Program::s_Instance->PushState(new ConnectGameState(player1, m_IsPlayer1AI, player2, m_IsPlayer2AI));
		}
	}

	std::string PlayState::CheckInputsAreValid(TextInput& username, TextInput& password, bool AI)
	{
		// Check for empty text input
		if (username.GetInput() == "")
			return "Please enter a username";

		// Check that user exists for both AI and Human
		if (!std::filesystem::exists("saves/" + username.GetInput()))
			return "User doesn't exist";

		// Only check password is correct for AI
		if (!AI)
		{
			Account user("saves/" + username.GetInput() + "/accountData.bin");
			if (!user.PasswordMatches(password.GetInput()))
				return "Password is incorrect";
		}

		return "";
	}
}