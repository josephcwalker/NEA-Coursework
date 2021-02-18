#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

#include "Program/Account.h"

#include <vector>

namespace Connect
{
	#define NUMBER_OF_SORTING_HEURISTICS 6

	enum SortHeuristic
	{
		NAME = 0,
		HUMAN_WINS,
		HUMAN_LOSSES,
		AI_WINS,
		AI_LOSSES,
		DATE_CREATED,
	};

	const std::string sortingHeuristics[NUMBER_OF_SORTING_HEURISTICS] =
	{
		"Name",
		"Human Wins",
		"Human Losses",
		"AI Wins",
		"AI Losses",
		"Date Created"
	};

	class LeaderboardState : public State
	{
	public:
		LeaderboardState()
			: m_BackButton(State::PopState, this),
			  m_SearchBar(this),
			  m_SortByButton(onClickFunction(SortByButtonFunction), this)

		{
			LOG_TRACE("Created Leaderboard State");
			LoadUsers();
		}
		virtual ~LeaderboardState() { LOG_TRACE("Deleting Leaderboard State"); }

	public:

		void Initialize() override
		{
			m_BackButton.SetPosition(sf::Vector2f(25.0f, 720.0f - 150.0f));
			m_BackButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_BackButton.SetText("Back");

			m_SearchBar.SetPosition(sf::Vector2f(700.0f, 25.0f));
			m_SearchBar.SetSize(sf::Vector2f(550.0f, 80.0f));
			m_SearchBar.SetText("Search: ");

			m_SortByButton.SetPosition(sf::Vector2f(700.0f, 125.0f));
			m_SortByButton.SetSize(sf::Vector2f(550.0f, 80.0f));
			m_SortByButton.SetText("Sort By: Name Ascending", true);

			m_Title.SetPosition(sf::Vector2f(80.0f, 50.0f));
			m_Title.SetCharacterSize(48);
			m_Title.SetText("Leader Board");

			m_UserInfo.SetPosition(sf::Vector2f(260.0f, 300.0f));
			m_UserInfo.SetCharacterSize(24);
			m_UserInfo.SetText("");
		}

		void Execute() override
		{
			m_SearchBar.KeyboardEvent();
			m_SearchBar.OnMouseUpdate(m_Window);

			m_SortByButton.OnMouseUpdate(m_Window);

			m_BackButton.OnMouseUpdate(m_Window);

			for (Button &b : m_UserButtons)
				b.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_Title);
			m_Window->draw(m_UserInfo);
			m_Window->draw(m_SearchBar);
			m_Window->draw(m_SortByButton);
			m_Window->draw(m_BackButton);

			for (Button &b : m_UserButtons)
				m_Window->draw(b);
		}
	
	private:
		void LoadUsers();
		void SortUsers();

		void UpdateUserButtons();

	// Button Functions
	private:
		void SortByButtonFunction();
		void UserButtonFunction();

	private:
		Text m_Title;
		Button m_BackButton;

		Text m_UserInfo;

		TextInput m_SearchBar;
		Button m_SortByButton;
		std::vector<Button> m_UserButtons;

		std::vector<Account> m_Users;

		SortHeuristic m_CurrentHeuristic = SortHeuristic::NAME;
		bool m_Ascending = true;
	};
}