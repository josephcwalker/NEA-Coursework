#include "LeaderboardState.h"

#include <filesystem>
#include <ctime>
#include <map>

namespace Connect
{
	void LeaderboardState::LoadUsers()
	{
		LOG_TRACE("Loading users from file");

		for (auto& p : std::filesystem::directory_iterator("saves/"))
			// Add user to list of all users
			m_Users.push_back((Account(p.path().generic_string() + "/accountData.bin")));

		SortUsers();
	}

	// Calculate the Levenshtein Distance between two strings
	int LevenshteinDistance(std::string left, std::string right)
	{
		// Matrix to store all distances for lengths of left and right
		std::vector<std::vector<int>> distanceMatrix(left.size() + 1);
		for (std::vector<int>& v : distanceMatrix)
			v.resize(right.size() + 1);

		// Distance from empty string to any string is just its length
		for (int i = 0; i < left.size() + 1; i++)
			distanceMatrix[i][0] = i;
		for (int i = 0; i < right.size() + 1; i++)
			distanceMatrix[0][i] = i;

		// Calculate distance using dynamic programming
		for (int i = 1, n = left.size() + 1; i < n; i++)
		{
			for (int j = 1, m = right.size() + 1; j < m; j++)
			{
				int substitutionCost = 0;
				if (left[i - 1] != right[j - 1])
					substitutionCost = 4;

				distanceMatrix[i][j] = std::min(
					{
						distanceMatrix[i-1][j] + 1,
						distanceMatrix[i][j-1] + 1,
						distanceMatrix[i-1][j-1] + substitutionCost
					});
			}
		}

		// Return distance of both full strings
		return distanceMatrix.back().back();
	}

	std::string SoundexConversion(std::string string)
	{
		char firstCharacter = string[0];
		static std::map<char, char> characterMap = {
			{'a', '0'}, {'e', '0'}, {'i', '0'}, {'o', '0'}, {'u', '0'},
			{'b', '1'}, {'f', '1'}, {'p', '1'}, {'v', '1'},
			{'c', '2'}, {'g', '2'}, {'j', '2'}, {'k', '2'}, {'q', '2'}, {'s', '2'}, {'x', '2'}, {'z', '2'},
			{'d', '3'}, {'t', '3'},
			{'l', '4'},
			{'m', '5'}, {'n', '5'},
			{'r', '6'}
		};

		// Replace all characters with their respective digit
		for (int i = 1; i < string.size(); i++)
			string[i] = characterMap[string[i]];

		// Remove repeated adjacent characters and 0s
		char lastCharacter = string[0];
		std::string newString(1, lastCharacter);
		for (int i = 1; i < string.size(); i++)
		{
			// If character is different from previous character
			if (string[i] != lastCharacter && string[i] != '0')
				newString.push_back(string[i]);

			lastCharacter = string[i];
		}

		// There are extra steps to the algorithm but the Levenshtein distance will fix most of them
		return newString;
	}

	int GetStringHeuristic(std::string string, std::string reference)
	{
		// Convert both string using Soundex then return the distance between those two strings
		return LevenshteinDistance(SoundexConversion(string), SoundexConversion(reference));
	}

	void LeaderboardState::SortUsers()
	{
		LOG_TRACE("Sorting users");

		std::string searchText = m_SearchBar.GetInput();

		// Function to compare two accounts to each other
		// If not using the search bar then just use default sorting method
		std::function<bool(Account &, Account &)> comparisonFunction;

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

		std::function<bool(Account &, Account &)> comparisonWithHeuristic = [comparisonFunction](Account &left, Account &right) { return comparisonFunction(left, right); };

		// If using search bar then compare by Levenshtein distance of Soundex form then sort by default method
		if (!searchText.empty())
		{
			comparisonWithHeuristic = [&](Account &left, Account &right) {
				// Recomputing heuristics multiple times possible optimization
				int leftHeuristic = GetStringHeuristic(left.GetName(), searchText);
				int rightHeuristic = GetStringHeuristic(right.GetName(), searchText);

				if (leftHeuristic == rightHeuristic)
					return comparisonFunction(left, right);
				return leftHeuristic < rightHeuristic;
			};
		}

		std::sort(m_Users.begin(), m_Users.end(), comparisonWithHeuristic);
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

	void LeaderboardState::KeyboardEventCallback()
	{
		// When the user types in the search box
		SortUsers();
		UpdateUserButtons();
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