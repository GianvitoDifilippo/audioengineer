#pragma once

#include "neuralnetwork.h"

namespace audioengineer {

	namespace utils {
		class FileUtils;
	}

	namespace core {

		class NetworkTrainer
		{
			friend class audioengineer::utils::FileUtils;

		private:
			uint m_iterations;
			uint m_batchSize;
			std::vector<std::pair<vec*, vec*>> m_trainingData;

		public:
			NetworkTrainer() = default;
			NetworkTrainer(uint iterations, uint batchSize = 1);
			NetworkTrainer(const NetworkTrainer& rhs);
			NetworkTrainer(NetworkTrainer&& rhs);
			~NetworkTrainer();

			uint getIterations() const;

			void setIterations(uint iterations);

			void addTrainingData(const vec& input, const vec& target);
			const std::pair<vec*, vec*>& getRandomTrainingSample() const;
			void train(NeuralNetwork& net);

			NetworkTrainer& operator=(NetworkTrainer rhs);

		private:
			void cleanup();
			void move(NetworkTrainer& rhs);
		};

		inline uint NetworkTrainer::getIterations() const
		{
			return m_iterations;
		}

		inline void NetworkTrainer::setIterations(uint iterations)
		{
			m_iterations = iterations;
		}

		inline const std::pair<vec*, vec*>& NetworkTrainer::getRandomTrainingSample() const
		{
			return m_trainingData[utils::randUint(0, m_trainingData.size())];
		}
	}
}