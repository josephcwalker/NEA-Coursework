#include "Account.h"

#include <fstream>
#include <filesystem>

#define NEURAL_NETWORK_LAYER_SIZES {100, 100, 100}

namespace Connect
{
	Account::Account(std::string name, std::string password)
		: m_Name(name), m_Password(password)
	{
		// Create new neural network with random initialization
		m_NeuralNetwork = new NeuralNetwork(NEURAL_NETWORK_LAYER_SIZES);

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
		file.read((char*)&passwordSize, 4);

		// Read username and password string
		file.seekg(bytesInMetadata);
		m_Name.resize(usernameSize);
		file.read(&m_Name[0], usernameSize);

		m_Password.resize(passwordSize);
		file.read(&m_Password[0], passwordSize);

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
		if (m_NeuralNetwork)
			delete m_NeuralNetwork;
	}

	void Account::LoadNeuralNetwork()
	{
		// Load the account from the file
		std::ifstream file("saves/" + m_Name + "/accountData.bin", std::ios::binary);

		if (!file.is_open())
		{
			LOG_ERROR("Cannot open file for loading neural network data");
			return;
		}

		// Navigate to neural network metadata
		file.seekg(3 * sizeof(unsigned int));

		// Read number of layers
		unsigned int numberOfLayers;
		file.read((char*)&numberOfLayers, sizeof(unsigned int));

		// Read and save each layer
		std::vector<unsigned int> layerSizes;
		layerSizes.resize(numberOfLayers);

		for (int i = 0; i < numberOfLayers; i++)
			file.read((char*)&layerSizes[i], sizeof(unsigned int));

		// Get size of file and navigate to neural network data
		file.seekg(0, std::ios::end);
		unsigned int sizeOfFile = file.tellg();
		file.seekg(m_NNOffset);

		// Read rest of file containing neural network data
		std::shared_ptr<char[]> data(new char[sizeOfFile - m_NNOffset]);
		file.read(data.get(), sizeOfFile - m_NNOffset);

		file.close();

		// Load neural network object
		m_NeuralNetwork = new NeuralNetwork(layerSizes, data);
		}

	void Account::SaveToFile()
	{
		if (!std::filesystem::exists("saves/" + m_Name))
			std::filesystem::create_directories("saves/" + m_Name);

		bool alreadyExists = std::filesystem::exists("saves/" + m_Name + "/accountData.bin");

		std::ofstream file("saves/" + m_Name + "/accountData.bin", std::ios::binary);

		if (!file.is_open())
		{
			LOG_ERROR("Cannot open file to save data to");
			return;
		}

		if (!alreadyExists)
		{
			// Problem! To write to a binary file and keep 4 byte long numberse
			// Must save number as a lvalue and then write that
			// Find a workaround

			// ----------- Write Metadata -----------
			std::vector<unsigned int> layerSizes = m_NeuralNetwork->GetLayerSizes();

			int sizeOfMetadata = 16 + layerSizes.size() * sizeof(unsigned int);
			file.write(reinterpret_cast<const char*>(&sizeOfMetadata), 4);

			int sizeOfUsername = m_Name.size();
			file.write(reinterpret_cast<const char*>(&sizeOfUsername), 4);

			int sizeOfPassword = m_Password.size();
			file.write(reinterpret_cast<const char*>(&sizeOfPassword), 4); // TODO: store digest size of SHA-1

			int numberOfLayers = layerSizes.size();
			file.write(reinterpret_cast<const char*>(&numberOfLayers), 4);

			for (int i = 0; i < numberOfLayers; i++)
				file.write(reinterpret_cast<const char*>(&layerSizes[i]), 4);

			// ----------- Write header -----------
			file.write(m_Name.c_str(), sizeOfUsername);
			file.write(m_Password.c_str(), sizeOfPassword); // TODO: Hash this using SHA-1

			file.write(reinterpret_cast<const char *>(&time), sizeof(time_t));
		}
		else
		{
			file.seekp(m_NNOffset - 2 * sizeof(WinLosses));
		}

		file.write(reinterpret_cast<const char*>(&HumanStats), sizeof(WinLosses));
		file.write(reinterpret_cast<const char*>(&AIStats), sizeof(WinLosses));

		// Write neural network data
		BinaryData neuralNetworkData = m_NeuralNetwork->Serialize();
		file.write(neuralNetworkData.first.get(), neuralNetworkData.second);

		file.close();
	}
}