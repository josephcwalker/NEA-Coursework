#include <Eigen/Dense>

#include <vector>

namespace Connect
{
	typedef std::pair<char*, size_t> BinaryData;
	typedef std::pair<Eigen::VectorXf, Eigen::VectorXf> TrainingExample;

	class NeuralNetwork
	{
	public:
		// Random weights and biases
		NeuralNetwork(std::vector<size_t> layerSizes);
		// Data from a file
		NeuralNetwork(std::vector<size_t> layerSizes, char* data);
		virtual ~NeuralNetwork();

	public:

		Eigen::VectorXf FeedFoward(Eigen::VectorXf inputVector);
		void Train(std::vector<TrainingExample>& trainingData, float eta);

		BinaryData Serialize();

	private:
		void RandomInitialize();
		
		void Backpropagation(std::vector<Eigen::MatrixXf>& nablaWeights, std::vector<Eigen::VectorXf>& nablaBiases, TrainingExample& example);

		Eigen::VectorXf activationFunctionVector(Eigen::VectorXf& vector);
		Eigen::VectorXf activationFunctionPrimeVector(Eigen::VectorXf& vector);

		static inline float activationFunction(float value);
		static inline float activationFunctionPrime(float value);

	private:
		std::vector<Eigen::MatrixXf> m_Weights;
		std::vector<Eigen::VectorXf> m_Biases;
	};
}