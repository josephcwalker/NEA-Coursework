#include "CreateAccountState.h"

#include <filesystem>
#include <fstream>
#include <cstring>

void Connect::CreateAccountState::CreateAccountButtonFunction()
{
	LOG_TRACE("Create Account Button Pressed");
	m_Messages.SetText("");

	// Check user doesn't already exist and then create account
	if (!std::filesystem::is_directory("saves"))
		std::filesystem::create_directories("saves");

	std::string error_message = CheckInputsAreValid();
	if (strcmp(error_message.c_str(), "") == 0)
	{
		// Create folder and account data file
		LOG_TRACE("Created account and stored data");
		EnterDataIntoFile();
		m_Messages.SetText("Successfully created user account " + m_UsernameInput.GetInput());
	}
	else
	{
		// Print error messages if they exist
		LOG_WARN("Error Creating user account");
		m_Messages.SetText("Error: " + error_message);
	}

}

void Connect::CreateAccountState::EnterDataIntoFile()
{
	// Open File
	std::filesystem::create_directories("saves/" + m_UsernameInput.GetInput());
	std::ofstream userFile("saves/" + m_UsernameInput.GetInput() + "/accountData.bin");

	// Write Metadata
	userFile << 8; // + neural network metadata
	userFile << m_UsernameInput.GetInput().size();
	userFile << m_PasswordInput.GetInput().size(); // TODO: store digest size of SHA-1
	// userFile << neural network metadata

	// Write header
	userFile << m_UsernameInput.GetInput();
	userFile << m_PasswordInput.GetInput(); // TODO: Hash this using SHA-1
	userFile << time(nullptr);
	userFile << 0 << 0; // Human Wins/Losses
	userFile << 0 << 0; // AI Wins/Losses
	// userFile << neural network data

	// Close File
	userFile.close();
}

std::string Connect::CreateAccountState::CheckInputsAreValid()
{
	std::string error_message = "";

	// Check if username is valid and exists
	if (strcmp(m_UsernameInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a username\n";
	else if (std::filesystem::is_directory("saves/" + m_UsernameInput.GetInput()))
		error_message += "Username already exists\n";

	// Check if password is valid and exists
	if (strcmp(m_PasswordInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a password\n";

	return error_message;
}