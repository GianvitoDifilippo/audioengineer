#pragma once

#include <algorithm>
#include <limits>
#include "networktrainer.h"

namespace audioengineer {

	namespace utils {
		class FileUtils;
	}

	namespace core {

		struct PopulationStats
		{
			float averageFitness;
			float bestFitness;
			float worstFitness;
			unsigned iteration;
		};

		class TrainingField
		{
			friend class audioengineer::utils::FileUtils;

		public:
			std::vector<NetworkTrainer> m_trainers;
			std::vector<NeuralNetwork> m_population;
			uint m_populationSize;
			float m_survivalRate;
			PopulationStats m_stats;
			Metaparameters m_parameters;
			uint m_inputCount;
			uint m_outputCount;
			NeuralNetwork m_best;
			NeuralNetwork m_allTimeBest;
			float m_allTimeBestFitness;

		public:
			TrainingField() = default;
			TrainingField(uint inputCount, uint outputCount, uint populationSize, const Metaparameters& parameters = {});

			void setSurvivalRate(float survivalRate);

			void addNetworkTrainer(const NetworkTrainer& trainer);
			void addNetworkTrainer(NetworkTrainer&& trainer);
			void addTrainingData(uint trainer, const std::vector<float>& input, const std::vector<float>& target);
			void generatePopulation();
			void simulate();

			float getSurvivalRate() const;
			uint getPopulationSize() const;
			const PopulationStats& getStats() const;
			const Metaparameters& getMetaparameters() const;

			const NeuralNetwork& getBestIndividual() const;
			const NeuralNetwork& getAllTimeBestIndividual() const;

		private:
			static bool sortingFcn(const std::pair<uint, float>& p1, const std::pair<uint, float>& p2);
		};
	}
}