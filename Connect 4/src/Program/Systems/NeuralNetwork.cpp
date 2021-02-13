#include "NeuralNetwork.h"

#include "Tools/Logger.h"

namespace Connect
{
	NeuralNetwork::NeuralNetwork(std::vector<size_t> layerSizes)
	{
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

	NeuralNetwork::NeuralNetwork(std::vector<size_t> layerSizes, char* data)
	{

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

	void NeuralNetwork::Train(std::vector<TrainingExample> &trainingData, float eta)
	{

	}

	BinaryData NeuralNetwork::Serialize()
	{
		return BinaryData();
	}

	void NeuralNetwork::RandomInitialize()
	{
		// Set all weights and biases to random values in a range
		for (int i = 0; i < m_Weights.size(); i++)
		{
			// Initialize between -epsilon and +epsilon
			float epsilon = sqrt(6 / (m_Weights[i].cols() + m_Weights[i].rows()));

			// Set random is in the range [-1, 1]
			m_Weights[i].setRandom();
			m_Biases[i].setRandom();

			// Multiply by epsilon to get range [-eps, eps]
			m_Weights[i] = epsilon * m_Weights[i];
			m_Biases[i] = epsilon * m_Biases[i];
		}

		LOG_TRACE("Neural network randomly initialized");
	}

	void NeuralNetwork::Backpropagation(std::vector<Eigen::MatrixXf> &nablaWeights, std::vector<Eigen::VectorXf> &nablaBiases, TrainingExample &example)
	{

	}

	Eigen::VectorXf NeuralNetwork::activationFunctionVector(Eigen::VectorXf& vector)
	{
		return vector.unaryExpr(std::function(activationFunction));
	}

	Eigen::VectorXf NeuralNetwork::activationFunctionPrimeVector(Eigen::VectorXf& vector)
	{
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