#include "Tools/Logger.h"

#include <Eigen/Dense>

#include <vector>

namespace Connect
{
	typedef std::pair<char*, size_t> BinaryData;
	typedef std::pair<Eigen::VectorXf, Eigen::VectorXf> TrainingExample;

	class NeuralNetwork
	{
	public:
		NeuralNetwork(std::vector<size_t> layerSizes);
		NeuralNetwork(std::vector<size_t> layerSizes, char* data);
		virtual ~NeuralNetwork();

	public:

		Eigen::VectorXf FeedFoward(Eigen::VectorXf& inputVector);
		void Train(std::vector<TrainingExample>& trainingData, float eta);

		BinaryData Serialize();

	private:
		void RandomIntialize();
		
		void Backpropagation(std::vector<Eigen::MatrixXf>& nablaWeights, std::vector<Eigen::VectorXf>& nablaBiases, TrainingExample& example);

		inline Eigen::VectorXf activationFunctionVector(Eigen::VectorXf& vector);
		inline Eigen::VectorXf activationFunctionPrimeVector(Eigen::VectorXf &vector);

		inline float activationFunction(float value);
		inline float activationFunctionPrime(float value);

	private:
		std::vector<Eigen::MatrixXf> m_Weights;
		std::vector<Eigen::VectorXf> m_Biases;
	};
}