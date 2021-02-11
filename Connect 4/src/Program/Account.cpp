#include "Account.h"

#include <fstream>
#include <filesystem>

namespace Connect
{
	Account::Account(std::string name, std::string password)
		: m_Name(name), m_Password(password)
	{
		// Create new neural network with random initialization
		// m_NN = new NN()

		DateCreated = time(nullptr);
	}

	Account::Account(std::string filename)
	{
		// Load the account from the file
		std::ifstream file(filename, std::ios::binary);

		if (!file.is_open())
		{
			LOG_ERROR("Cannot open file for creating account object");
			return;
		}

		// Read bytes in metadata
		int bytesInMetadata;
		file.read((char*)&bytesInMetadata, 4);

		// Read username and password size
		int usernameSize;
		file.read((char*)&usernameSize, 4);
		int passwordSize;
		file.read((char *)&passwordSize, 4);

		// Read NN metadata

		// Read username and password string
		file.seekg(bytesInMetadata);
		m_Name.resize(usernameSize + 1);
		file.read(&m_Name[0], usernameSize);
		m_Name[usernameSize] = '\0';

		m_Password.resize(passwordSize + 1);
		file.read(&m_Password[0], passwordSize);
		m_Password[passwordSize] = '\0';

		// Read date/time created
		file.read((char*)&DateCreated, sizeof(time_t));

		// Read human/ai stats
		file.read((char*)&HumanStats, sizeof(WinLosses));
		file.read((char*)&AIStats, sizeof(WinLosses));

		m_NNOffset = file.tellg();
		file.close();
	}

	Account::~Account()
	{

	}

	void Account::SaveToFile()
	{
		if (!std::filesystem::exists("saves/" + m_Name))
			std::filesystem::create_directories("saves/" + m_Name);

		bool alreadyExists = std::filesystem::exists("saves/" + m_Name + "/accountData.bin");

		std::ofstream file("saves/" + m_Name + "/accountData.bin", std::ios::binary);

		if (!alreadyExists)
		{
			// Problem! To write to a binary file and keep 4 byte long numberse
			// Must save number as a lvalue and then write that
			// Find a workaround

			// ----------- Write Metadata -----------
			int sizeOfMetadata = 12; // + neural network metadata
			file.write(reinterpret_cast<const char *>(&sizeOfMetadata), 4);

			int sizeOfUsername = m_Name.size();
			file.write(reinterpret_cast<const char *>(&sizeOfUsername), 4);

			int sizeOfPassword = m_Password.size();
			file.write(reinterpret_cast<const char *>(&sizeOfPassword), 4); // TODO: store digest size of SHA-1

			// userFile << neural network metadata

			// ----------- Write header -----------
			file.write(m_Name.c_str(), sizeOfUsername);
			file.write(m_Password.c_str(), sizeOfPassword); // TODO: Hash this using SHA-1

			file.write(reinterpret_cast<const char *>(&time), sizeof(time_t));
		}
		else
		{
			file.seekp(m_NNOffset);
		}

		file.write(reinterpret_cast<const char*>(&HumanStats), sizeof(WinLosses));
		file.write(reinterpret_cast<const char*>(&AIStats), sizeof(WinLosses));

		// Write neural network data

		file.close();
	}
}