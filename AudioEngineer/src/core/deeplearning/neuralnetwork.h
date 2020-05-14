#pragma once

#include "layer.h"
#include "joint.h"

namespace audioengineer {

	namespace utils {
		class FileUtils;
	}

	namespace core {

		struct Metaparameters
		{
			float survivalRateMin = 0.3f;
			float survivalRateMax = 0.7f;
			float learningRateMin = 0.01f;
			float learningRateMax = 2.f;
			float mutationProbability = 0.01f;
			uint hiddenLayerCountMin = 0;
			uint hiddenLayerCountMax = 4;
			uint neuronCountMin = 2;
			uint neuronCountMax = 40;
			uint testSampleCount = 10;
		};

		class NeuralNetwork
		{
			friend class audioengineer::utils::FileUtils;

		private:
			std::vector<uint> m_topology;
			std::vector<Layer> m_layers;
			std::vector<Joint> m_joints;
			vec m_derSigma;
			float m_learningRate;

		public:
			NeuralNetwork() = default;
			NeuralNetwork(const std::vector<uint>& topology, float learningRate);

			const std::vector<uint>& getTopology() const;
			float getLearningRate() const;

			void setLearningRate(float learningRate);

			void feed(const vec& input);
			vec operator()(const vec& input);
			float getMSE(const vec& input, const vec& target);
			float getMSE();

			void computeStep(const vec& input, const vec& target);
			void takeStep(uint trainingCount);

			NeuralNetwork breed(const Metaparameters& params, float fitness);
		};

		inline float sigmoid(float x)
		{
			return 1.f / (1.f + exp(-x));
		}
	}
}