#include <Eigen/Dense>

#include <vector>
#include <memory>

namespace Connect
{
	typedef std::pair<std::shared_ptr<char[]>, size_t> BinaryData;
	typedef std::pair<Eigen::VectorXf, Eigen::VectorXf> TrainingExample;

	class NeuralNetwork
	{
	public:
		// Random weights and biases
		NeuralNetwork(std::vector<size_t> layerSizes);
		// Data from a file
		NeuralNetwork(std::vector<unsigned int>& layerSizes, std::shared_ptr<char[]> data);
		virtual ~NeuralNetwork();

	public:
		Eigen::VectorXf FeedFoward(Eigen::VectorXf inputVector);

		BinaryData Serialize();

		std::vector<unsigned int> GetLayerSizes();

		// Returns the accuracy of the network lower = better
		float Train(std::vector<TrainingExample>& trainingData, float eta);

	private:
		float Backpropagation(std::vector<Eigen::MatrixXf>& nablaWeights, std::vector<Eigen::VectorXf>& nablaBiases, TrainingExample& example);

		void RandomInitialize();

		Eigen::VectorXf activationFunctionVector(Eigen::VectorXf& vector);
		Eigen::VectorXf activationFunctionPrimeVector(Eigen::VectorXf& vector);

		static inline float activationFunction(float value);
		static inline float activationFunctionPrime(float value);

	private:
		std::vector<Eigen::MatrixXf> m_Weights;
		std::vector<Eigen::VectorXf> m_Biases;
	};
}