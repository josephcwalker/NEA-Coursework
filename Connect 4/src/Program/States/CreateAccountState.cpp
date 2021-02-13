#include "CreateAccountState.h"

#include "Program/Account.h"

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
		EnterDataIntoFile();
		LOG_TRACE("Created account and stored data");
		m_Messages.SetText("Successfully created user account " + m_UsernameInput.GetInput());
	}
	else
	{
		// Print error messages if they exist
		LOG_WARN("Problem in creating user account");
		m_Messages.SetText("Error: " + error_message);
	}
}

void Connect::CreateAccountState::EnterDataIntoFile()
{
	Account account(m_UsernameInput.GetInput(), m_PasswordInput.GetInput());
	account.SaveToFile();
}

std::string Connect::CreateAccountState::CheckInputsAreValid()
{
	std::string error_message = "";

	// Check if username is valid and doesn't exist
	if (strcmp(m_UsernameInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a username\n";
	else if (std::filesystem::is_directory("saves/" + m_UsernameInput.GetInput()))
		error_message += "Username already exists\n";

	// Check if password is valid and exists
	if (strcmp(m_PasswordInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a password\n";

	return error_message;
}