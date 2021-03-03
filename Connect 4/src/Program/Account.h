#pragma once

#include "Tools/Logger.h"

#include "Systems/NeuralNetwork.h"

#include <string>

namespace Connect
{
	struct WinLosses
	{
		int wins = 0;
		int losses = 0;
	};

	class Account
	{
	public:
		Account(std::string name, std::string password);
		Account(std::string filename);
		virtual ~Account();

	public:

		// Doesn't have to load NN if not necessary
		void LoadNeuralNetwork();

		void SaveToFile();

		inline bool PasswordMatches(std::string testPassword) { return strcmp(m_Password.c_str(), testPassword.c_str()) == 0; }
		virtual inline const std::string& GetName() const { return m_Name; }

	public:
		WinLosses HumanStats;
		WinLosses AIStats;

		time_t DateCreated;

	private:
		std::string m_Name;
		std::string m_Password;

		// Used to index navigate straight to where the NN is stored
		unsigned int m_NNOffset;

	protected:
		NeuralNetwork* m_NeuralNetwork;
	};
}