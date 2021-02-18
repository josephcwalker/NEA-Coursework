#include "LeaderboardState.h"

#include <filesystem>
#include <ctime>

namespace Connect
{
	void LeaderboardState::LoadUsers()
	{
		LOG_TRACE("Loading users from file");

		for (auto& p : std::filesystem::directory_iterator("saves/"))
			// Add user to list of all users
			m_Users.push_back(Account(p.path().generic_string() + "/accountData.bin"));

		SortUsers();
	}

	void LeaderboardState::SortUsers()
	{
		LOG_TRACE("Sorting users");
		std::function<bool(Account&, Account&)> comparisonFunction;

		// Determine sorting function based on heuristic
		switch (m_CurrentHeuristic)
		{
		case SortHeuristic::NAME:
			comparisonFunction = [](Account& left, Account& right){ return strcmp(left.GetName().c_str(), right.GetName().c_str()) != 1; };
			break;
		case SortHeuristic::HUMAN_WINS:
			comparisonFunction = [](Account &left, Account &right) { return left.HumanStats.wins < right.HumanStats.wins; };
			break;
		case SortHeuristic::HUMAN_LOSSES:
			comparisonFunction = [](Account &left, Account &right) { return left.HumanStats.losses < right.HumanStats.losses; };
			break;
		case SortHeuristic::AI_WINS:
			comparisonFunction = [](Account &left, Account &right) { return left.AIStats.wins < right.AIStats.wins; };
			break;
		case SortHeuristic::AI_LOSSES:
			comparisonFunction = [](Account &left, Account &right) { return left.AIStats.losses < right.AIStats.losses; };
			break;
		case SortHeuristic::DATE_CREATED:
			comparisonFunction = [](Account &left, Account &right) { return left.DateCreated < right.DateCreated; };
			break;
		default:
			LOG_ERROR("Invalid sorting heuristic used");
			return;
			break;
		}

		std::sort(m_Users.begin(), m_Users.end(), comparisonFunction);
		if (!m_Ascending)
			std::reverse(m_Users.begin(), m_Users.end());
		
		LOG_TRACE("Finished sorting users");

		UpdateUserButtons();
	}

	void LeaderboardState::UpdateUserButtons()
	{
		// Display top 5 users
		for (int i = 0; i < 5 && i < m_Users.size(); i++)
		{
			// Check whether the button at that position already exists
			if (i >= m_UserButtons.size())
			{
				m_UserButtons.push_back(Button(onClickFunction(UserButtonFunction), this));
				m_UserButtons[i].SetPosition(sf::Vector2f(825.0f, 225.0f + i * 100.0f));
				m_UserButtons[i].SetSize(sf::Vector2f(425.0f, 80.0f));
			}

			m_UserButtons[i].SetText(m_Users[i].GetName(), true);
			m_UserButtons[i].id = i;
		}
	}

	void LeaderboardState::SortByButtonFunction()
	{
		// Change sorting method
		if (!m_Ascending)
		{
			m_Ascending = true;
			m_CurrentHeuristic = static_cast<SortHeuristic>((m_CurrentHeuristic + 1) % NUMBER_OF_SORTING_HEURISTICS);
		}
		else
			m_Ascending = false;

		m_SortByButton.SetText("Sort By: " + sortingHeuristics[m_CurrentHeuristic] + (m_Ascending ? " Ascending" : " Descending"), true);
		LOG_TRACE("Changed Sorting heuristic");
		SortUsers();
	}

	void LeaderboardState::UserButtonFunction()
	{
		LOG_TRACE("User clicked");

		// Display All Info about clicked user
		Account& user = m_Users[recentButton->id];

		std::string info = user.GetName();
		info += "\n";
		info += "\nHuman Wins: " + std::to_string(user.HumanStats.wins);
		info += "\nHuman Losses: " + std::to_string(user.HumanStats.losses);
		info += "\nAI Wins: " + std::to_string(user.AIStats.wins);
		info += "\nAI Losses: " + std::to_string(user.AIStats.losses);
		info += "\n\n";

		char buffer[30];
		strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", std::localtime(&user.DateCreated));
		info += "Date Created: " + std::string(buffer);

		m_UserInfo.SetText(info);
	}
}