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
		LOG_WARN("Problem in creating user account");
		m_Messages.SetText("Error: " + error_message);
	}
}

void Connect::CreateAccountState::EnterDataIntoFile()
{
	// Open File
	std::filesystem::create_directories("saves/" + m_UsernameInput.GetInput());
	std::ofstream userFile("saves/" + m_UsernameInput.GetInput() + "/accountData.bin", std::ios::binary);

	// Problem! To write to a binary file and keep 4 byte long numberse
	// Must save number as a lvalue and then write that
	// Find a workaround

	// ----------- Write Metadata -----------
	int sizeOfMetadata = 12; // + neural network metadata
	userFile.write(reinterpret_cast<const char *>(&sizeOfMetadata), 4);

	int sizeOfUsername = m_UsernameInput.GetInput().size();
	userFile.write(reinterpret_cast<const char*>(&sizeOfUsername), 4);

	int sizeOfPassword = m_PasswordInput.GetInput().size();
	userFile.write(reinterpret_cast<const char *>(&sizeOfPassword), 4); // TODO: store digest size of SHA-1

	// userFile << neural network metadata

	// ----------- Write header -----------
	userFile.write(m_UsernameInput.GetInput().c_str(), sizeOfUsername);
	userFile.write(m_PasswordInput.GetInput().c_str(), sizeOfPassword); // TODO: Hash this using SHA-1

	time_t currentTime = time(nullptr);
	userFile.write(reinterpret_cast<const char*>(&time), sizeof(time_t));

	// Write human and AI wins and losses
	int zero = 0;
	for (int i = 0; i < 4; i++)
		userFile.write(reinterpret_cast<const char*>(&zero), 4);

	// userFile << neural network data

	// Close File
	userFile.close();
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