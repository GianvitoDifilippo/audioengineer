#include "trainingfield.h"

namespace audioengineer {
	namespace core {

		TrainingField::TrainingField(uint inputCount, uint outputCount, uint populationSize, const Metaparameters& parameters)
		{
			m_populationSize = populationSize;
			m_parameters = parameters;
			m_inputCount = inputCount;
			m_outputCount = outputCount;
			m_population.reserve(populationSize);
			for (uint i = 0; i < m_populationSize; i++) {
				uint layerCount = utils::randUint(parameters.hiddenLayerCountMin, parameters.hiddenLayerCountMax);
				std::vector<uint> topology(layerCount + 2);
				topology[0] = inputCount;
				for (uint j = 0; j < layerCount; j++) {
					topology[j + 1] = utils::randUint(parameters.neuronCountMin, parameters.neuronCountMax);
				}
				topology[layerCount + 1] = outputCount;
				m_population.emplace_back(topology, utils::randFloat(parameters.learningRateMin, parameters.learningRateMax));
			}
			m_survivalRate = 0.5f;
			m_allTimeBestFitness = 0.f;
			m_best = m_population[utils::randUint(0, m_populationSize)];
			m_allTimeBest = m_population[utils::randUint(0, m_populationSize)];
		}

		void TrainingField::setSurvivalRate(float survivalRate)
		{
			m_survivalRate = survivalRate;
			if (m_survivalRate > m_parameters.survivalRateMax) {
				m_survivalRate = m_parameters.survivalRateMax;
			}
			if (m_survivalRate < m_parameters.survivalRateMin) {
				m_survivalRate = m_parameters.survivalRateMin;
			}
		}

		void TrainingField::addNetworkTrainer(const NetworkTrainer& trainer)
		{
			m_trainers.push_back(trainer);
		}

		void TrainingField::addNetworkTrainer(NetworkTrainer&& trainer)
		{
			m_trainers.push_back(std::move(trainer));
		}

		void TrainingField::addTrainingData(uint trainer, const std::vector<float>& input, const std::vector<float>& target)
		{
			if (trainer >= m_trainers.size()) return;
			m_trainers[trainer].addTrainingData(input, target);
		}

		void TrainingField::generatePopulation()
		{
			m_stats.iteration = 0;
			for (uint i = 0; i < m_populationSize; i++) {
				uint layerCount = utils::randUint(m_parameters.hiddenLayerCountMin, m_parameters.hiddenLayerCountMax);
				std::vector<uint> topology(layerCount + 2);
				topology[0] = m_inputCount;
				for (uint j = 0; j < layerCount; j++) {
					topology[j + 1] = utils::randUint(m_parameters.neuronCountMin, m_parameters.neuronCountMax);
				}
				topology[layerCount + 1] = m_outputCount;
				m_population[i] = NeuralNetwork(topology, utils::randFloat(m_parameters.learningRateMin, m_parameters.learningRateMax));
			}
		}

		void TrainingField::simulate()
		{
			NetworkTrainer& trainer = m_trainers[utils::randUint(0, m_trainers.size())];
			std::vector<std::pair<uint, float>> fitnesses(m_populationSize);
			float previousAverageFitness = m_stats.averageFitness;
			m_stats = { 0.f, 0.f, std::numeric_limits<float>::max(), m_stats.iteration };
			for (uint i = 0; i < m_populationSize; i++) {
				trainer.train(m_population[i]);
				float fitnessTest = 0.f;
				for (uint j = 0; j < m_parameters.testSampleCount; j++) {
					const std::pair<vec*, vec*>& sample = trainer.getRandomTrainingSample();
					float mse = m_population[i].getMSE(*sample.first, *sample.second);
					fitnessTest += (mse - fitnessTest) / (float)(j + 1);
				}
				std::pair<uint, float> fitness = std::make_pair(i, 1.f / sqrt(fitnessTest));
				if (fitness.second == std::numeric_limits<float>::infinity()) {
					fitness.second = std::numeric_limits<float>::max();
				}
				fitnesses[i] = fitness;
				m_stats.averageFitness += (fitness.second - m_stats.averageFitness) / (float)(i + 1);
				if (fitness.second > m_stats.bestFitness) m_stats.bestFitness = fitness.second;
				if (fitness.second < m_stats.worstFitness) m_stats.worstFitness = fitness.second;
			}
			float invRange = 1.f / (m_stats.bestFitness - m_stats.worstFitness);
			uint killings = (uint)floor((1.f - m_survivalRate) * (float)m_populationSize);
			std::sort(fitnesses.begin(), fitnesses.end(), sortingFcn);
			std::vector<uint> toKill(killings);
			for (uint i = 0; i < killings; i++) {
				toKill[i] = fitnesses[fitnesses.size() - i - 1].first;
			}
			m_best = m_population[fitnesses[0].first];
			if (fitnesses[0].second > m_allTimeBestFitness) {
				m_allTimeBest = m_population[fitnesses[0].first];
				m_allTimeBestFitness = fitnesses[0].second;
			}
			float deltaFitness = m_stats.averageFitness - previousAverageFitness;
			if (m_stats.iteration > 10) {
				if (deltaFitness > 0) {
					m_survivalRate = (3.f * m_survivalRate + m_parameters.survivalRateMax) / 4.f;
				}
				else {
					m_survivalRate = (3.f * m_survivalRate + m_parameters.survivalRateMin) / 4.f;
				}
			}
			for (uint i = 0; i < killings; i++) {
				if (utils::rand01() < m_parameters.mutationProbability) {
					uint layerCount = utils::randUint(m_parameters.hiddenLayerCountMin, m_parameters.hiddenLayerCountMax);
					std::vector<uint> topology(layerCount + 2);
					topology[0] = m_inputCount;
					for (uint j = 0; j < layerCount; j++) {
						topology[j + 1] = utils::randUint(m_parameters.neuronCountMin, m_parameters.neuronCountMax);
					}
					topology[layerCount + 1] = m_outputCount;
					m_population[toKill[i]] = NeuralNetwork(topology, utils::randFloat(m_parameters.learningRateMin, m_parameters.learningRateMax));
				}
				else {
					uint index = utils::randUint(0, m_populationSize - killings);
					m_population[toKill[i]] = m_population[fitnesses[index].first].breed(m_parameters, fitnesses[toKill[i]].second);
				}
			}
			m_stats.iteration++;
		}

		float TrainingField::getSurvivalRate() const
		{
			return m_survivalRate;
		}

		uint TrainingField::getPopulationSize() const
		{
			return m_populationSize;
		}

		const PopulationStats& TrainingField::getStats() const
		{
			return m_stats;
		}

		const Metaparameters& TrainingField::getMetaparameters() const
		{
			return m_parameters;
		}

		const NeuralNetwork& TrainingField::getBestIndividual() const
		{
			return m_best;
		}
		
		const NeuralNetwork& TrainingField::getAllTimeBestIndividual() const
		{
			return m_allTimeBest;
		}

		bool TrainingField::sortingFcn(const std::pair<uint, float>& p1, const std::pair<uint, float>& p2)
		{
			return p1.second > p2.second;
		}

	}
}