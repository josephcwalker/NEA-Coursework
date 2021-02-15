#include "DeleteAccountState.h"

#include "Program/Account.h"

#include <filesystem>
#include <fstream>
#include <cstring>

void Connect::DeleteAccountState::DeleteAccountButtonFunction()
{
	LOG_TRACE("Delete Account Button Pressed")
	m_Messages.SetText("");

	std::string error_message = CheckInputsAreValid();
	if (strcmp(error_message.c_str(), "") == 0)
	{
		// No errors occurred so delete the specified user
		LOG_TRACE("Deleting user account");
		std::filesystem::remove_all("saves/" + m_UsernameInput.GetInput());

		m_Messages.SetText("Succesfully deleted user account " + m_UsernameInput.GetInput());
	}
	else
	{
		// Errors did occur, print them to the user
		LOG_WARN("Problem in deleting user account");
		m_Messages.SetText("Error: " + error_message);
	}
}

std::string Connect::DeleteAccountState::CheckInputsAreValid()
{
	std::string error_message = "";

	// Check username is valid and exists
	if (strcmp(m_UsernameInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a username\n";
	else if (!std::filesystem::is_directory("saves/" + m_UsernameInput.GetInput()))
		error_message += "User does not exist\n";

	if (!error_message.empty())
		return error_message;

	// Check for valid password
	if (strcmp(m_PasswordInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a password";
	else
	{
		// Check if password matches
		Account account("saves/" + m_UsernameInput.GetInput() + "/accountData.bin");

		if (!account.PasswordMatches(m_PasswordInput.GetInput() + '\0'))
			error_message += "Incorrect password\n";
	}

	return error_message;
}