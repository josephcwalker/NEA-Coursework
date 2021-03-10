#include "NeuralNetwork.h"

#include "Tools/Logger.h"

namespace Connect
{
	NeuralNetwork::NeuralNetwork(std::vector<size_t> layerSizes)
	{
		// Resize array to correct size
		m_Weights.reserve(layerSizes.size() - 1);
		m_Biases.reserve(layerSizes.size() - 1);

		for (int i = 1; i < layerSizes.size(); i++)
		{
			m_Weights.emplace_back(Eigen::MatrixXf(layerSizes[i], layerSizes[i - 1]));
			m_Biases.emplace_back(Eigen::VectorXf(layerSizes[i]));
		}

		LOG_TRACE("Neural Network object created");

		RandomInitialize();
	}

	NeuralNetwork::NeuralNetwork(std::vector<unsigned int> &layerSizes, std::shared_ptr<char[]> data)
	{
		char* pointer = data.get();

		// Reserve enough space for all layers
		m_Weights.reserve(layerSizes.size() - 1);
		m_Biases.reserve(layerSizes.size() - 1);

		for (int i = 1; i < layerSizes.size(); i++)
		{
			m_Weights.emplace_back(Eigen::MatrixXf(layerSizes[i], layerSizes[i - 1]));
			m_Biases.emplace_back(Eigen::VectorXf(layerSizes[i]));

			for (int j = 0; j < layerSizes[i - 1]; j++)
			{
				// Copy each column of data into matrix
				memcpy(&m_Weights[i - 1].col(j)[0], pointer, layerSizes[i] * sizeof(float));
				pointer += layerSizes[i] * sizeof(float);
			}

			// Copy entire bias vector
			memcpy(&m_Biases[i - 1][0], pointer, layerSizes[i] * sizeof(float));
			pointer += layerSizes[i] * sizeof(float);
		}

		LOG_TRACE("Constructed neural network from serialized data");
	}

	NeuralNetwork::~NeuralNetwork()
	{

	}

	Eigen::VectorXf NeuralNetwork::FeedFoward(Eigen::VectorXf inputVector)
	{
		// Multiply input vector by weights and add biase for each layer
		for (int i = 0; i < m_Weights.size(); i++)
		{
		#ifdef DEBUG
			if (m_Weights[i].cols() != inputVector.rows())
			{
				LOG_ERROR("Incorrect dimensions of matrix vector multiplication");
				return inputVector;
			}
		#endif

			inputVector = m_Weights[i] * inputVector + m_Biases[i];
			inputVector = activationFunctionVector(inputVector);
		}

		LOG_TRACE("Completed feed forward algorithm");
		return inputVector;
	}

	BinaryData NeuralNetwork::Serialize()
	{
		// Serializes the neural network weights and biases
		// into a byte array to be stored in accountData.bin

		size_t numberOfBytes = 0;

		// For each layer count number of bytes used
		for (int i = 0; i < m_Weights.size(); i++)
		{
			numberOfBytes += sizeof(float) * m_Weights[i].rows() * m_Weights[i].cols();
			numberOfBytes += sizeof(float) * m_Biases[i].rows();
		}

		std::shared_ptr<char[]> data(new char[numberOfBytes]);
		char* pointer = data.get();;

		// For each layer save weights and biases in data array
		for (int i = 0; i < m_Weights.size(); i++)
		{
			int previousNeurons = m_Weights[i].cols();
			int currentNeurons = m_Weights[i].rows();

			// For each column read every row into array
			for (int j = 0; j < previousNeurons; j++)
			{
				memcpy(pointer, &m_Weights[i].col(j)[0], currentNeurons * sizeof(float));
				pointer += currentNeurons * sizeof(float);
			}

			// Save bias vector in data array
			memcpy(pointer, &m_Biases[i][0], currentNeurons * sizeof(float));
			pointer += currentNeurons * sizeof(float);
		}

		LOG_TRACE("Serialized neural network data");

		return BinaryData(data, numberOfBytes);
	}

	std::vector<unsigned int> NeuralNetwork::GetLayerSizes()
	{
		// Count neurons in each layer and return
		std::vector<unsigned int> layerSizes(m_Weights.size() + 1);
		for (int i = 0; i < m_Weights.size(); i++)
			layerSizes[i + 1] = m_Weights[i].rows();
		layerSizes[0] = m_Weights[0].cols();

		return layerSizes;
	}

	float NeuralNetwork::Train(std::vector<TrainingExample>& trainingData, float eta)
	{
		// This is slow so only do it when compiling for debug
		#ifdef DEBUG
		for (int i = 0; i < trainingData.size(); i++)
		{
			// Check example input and output has same neurons as network
			if (trainingData[i].first.rows() != m_Weights[0].cols() || trainingData[i].second.rows() != m_Weights.back().rows())
			{
				LOG_ERROR("Incorrect matrix dimensions in training of neural network");
				return NAN;
			}
		}
		#endif

		// Changes to all weights and biases
		std::vector<Eigen::MatrixXf> nablaWeights(m_Weights.size());
		std::vector<Eigen::VectorXf> nablaBiases(m_Biases.size());

		// Initialize all nablas to 0
		for (int i = 0; i < m_Weights.size(); i++)
		{
			nablaWeights[i] = Eigen::MatrixXf::Zero(m_Weights[i].rows(), m_Weights[i].cols());
			nablaBiases[i] = Eigen::VectorXf::Zero(m_Biases[i].rows());
		}

		float cost = 0.0f;

		// Backpropagation happens here (updating nabla weights and nabla biases)
		for (TrainingExample& example : trainingData)
			cost += Backpropagation(nablaWeights, nablaBiases, example);
		
		// Update weights and biases based on calculated nablas
		float scalar = eta / trainingData.size();

		for (int i = 0; i < m_Weights.size(); i++)
		{
			m_Weights[i] -= scalar * nablaWeights[i];
			m_Biases[i] -= scalar * nablaBiases[i];
		}

		cost /= (2 * trainingData.size());

		LOG_TRACE("Trained neural network on training data");
		LOG_TRACE("Training neural network produced a cost of: " + std::to_string(cost));
		return cost;
	}

	float NeuralNetwork::Backpropagation(std::vector<Eigen::MatrixXf>& nablaWeights, std::vector<Eigen::VectorXf>& nablaBiases, TrainingExample& example)
	{
		// Temporary changes to all weights and biases
		std::vector<Eigen::MatrixXf> deltaWeights(m_Weights.size());
		std::vector<Eigen::VectorXf> deltaBiases(m_Biases.size());

		// Initialize all deltas to 0
		for (int i = 0; i < m_Weights.size(); i++)
		{
			deltaWeights[i] = Eigen::MatrixXf::Zero(m_Weights[i].rows(), m_Weights[i].cols());
			deltaBiases[i] = Eigen::VectorXf::Zero(m_Biases[i].rows());
		}

		// Feed forward and save all activation and z values
		std::vector<Eigen::VectorXf> activations(m_Weights.size() + 1);
		std::vector<Eigen::VectorXf> zvalues(m_Weights.size() + 1);

		activations[0] = example.first;

		for (int i = 0; i < m_Weights.size(); i++)
		{
			zvalues[i + 1] = m_Weights[i] * activations[i] + m_Biases[i];
			activations[i + 1] = activationFunctionVector(zvalues[i + 1]);
			zvalues[i + 1] = activationFunctionPrimeVector(zvalues[i + 1]);
		}

		// Backpropagate through the network calculating the changes to the weights and biases
		Eigen::VectorXf costVector = (activations.back() - example.second);

		float cost = pow(costVector.sum(), 2);
		costVector = costVector.array() * zvalues.back().array();

		deltaBiases[deltaBiases.size() - 1] = costVector;
		deltaWeights[deltaWeights.size() - 1] = costVector * activations[activations.size() - 2].transpose();

		for (int i = m_Weights.size() - 2; i >= 0; i--)
		{
			costVector = m_Weights[i + 1].transpose() * costVector;
			costVector = costVector.array() * zvalues[i + 1].array();

			deltaBiases[i] = costVector;
			deltaWeights[i] = costVector * activations[i].transpose();
		}

		// Update nabla weights and biases for total change to weights and biases
		for (int i = 0; i < deltaWeights.size(); i++)
		{
			nablaWeights[i] += deltaWeights[i];
			nablaBiases[i] += deltaBiases[i];
		}

		return cost;
	}

	void NeuralNetwork::RandomInitialize()
	{
		// Set all weights and biases to random values in a range
		for (int i = 0; i < m_Weights.size(); i++)
		{
			// Initialize between -epsilon and +epsilon
			float epsilon = sqrt(6.0f / (m_Weights[i].cols() + m_Weights[i].rows()));

			// Set random is in the range [-1, 1]
			m_Weights[i].setRandom();
			m_Biases[i].setRandom();

			// Multiply by epsilon to get range [-eps, eps]
			m_Weights[i] = epsilon * m_Weights[i];
			m_Biases[i] = epsilon * m_Biases[i];
		}

		LOG_TRACE("Neural network randomly initialized");
	}

	Eigen::VectorXf NeuralNetwork::activationFunctionVector(Eigen::VectorXf& vector)
	{
		// Apply activation function to an entire vector
		return vector.unaryExpr(std::function(activationFunction));
	}

	Eigen::VectorXf NeuralNetwork::activationFunctionPrimeVector(Eigen::VectorXf& vector)
	{
		// Apply activation function derivative to an entire vector
		return vector.unaryExpr(std::function(activationFunctionPrime));
	}

	float NeuralNetwork::activationFunction(float value)
	{
		// Sigmoid Activation Function
		// 1 / (1 + e^-x)
		return 1.0f / (1.0f + exp(-value));
	}

	float NeuralNetwork::activationFunctionPrime(float value)
	{
		// Derivative of the activation function
		float sigmoid = activationFunction(value);
		return sigmoid * (1 - sigmoid);
	}
}