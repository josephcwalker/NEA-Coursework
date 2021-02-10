#include "DeleteAccountState.h"

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

	// Check for valid password
	if (strcmp(m_PasswordInput.GetInput().c_str(), "") == 0)
		error_message += "Please enter a password";
	else
	{
		std::ifstream file("saves/" + m_UsernameInput.GetInput() + "/accountData.bin", std::ios::binary);

		if (!file.is_open())
			LOG_ERROR("File did not open when deleting account");

		// Read size of metadata and username size
		size_t* bytesInMetadata = new size_t();
		file.read((char*)bytesInMetadata, 4);
		unsigned int* usernameSize = new unsigned int();
		file.read((char*)usernameSize, 4);

		// Read password size
		unsigned int* passwordSize = new unsigned int();
		file.read((char*)passwordSize, 4);

		// Navigate to password and read
		char* password = new char[*passwordSize + 1];
		file.seekg(*bytesInMetadata + *usernameSize);
		file.read(password, *passwordSize);
		password[*passwordSize] = '\0';

		file.close();

		if (strcmp(m_PasswordInput.GetInput().c_str(), password) != 0)
			error_message += "Incorrect password\n";

		// Clean up - maybe replace with smart pointers
		delete bytesInMetadata;
		delete usernameSize;
		delete passwordSize;
		delete[] password;
	}

	return error_message;
}