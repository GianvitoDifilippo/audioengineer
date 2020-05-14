#include "networktrainer.h"

namespace audioengineer {
	namespace core {

		NetworkTrainer::NetworkTrainer(uint iterations, uint batchSize)
		{
			m_iterations = iterations;
			m_batchSize = batchSize;
		}

		NetworkTrainer::NetworkTrainer(const NetworkTrainer& rhs)
		{
			m_iterations = rhs.m_iterations;
			m_batchSize = rhs.m_batchSize;
			m_trainingData.reserve(rhs.m_trainingData.size());
			for (uint i = 0; i < rhs.m_trainingData.size(); i++) {
				vec* input = rhs.m_trainingData[i].first;
				vec* target = rhs.m_trainingData[i].second;
				m_trainingData.emplace_back(std::make_pair(new vec(*input), new vec(*target)));
			}
		}

		NetworkTrainer::NetworkTrainer(NetworkTrainer&& rhs)
		{
			move(rhs);
		}

		NetworkTrainer::~NetworkTrainer()
		{
			cleanup();
		}

		void NetworkTrainer::addTrainingData(const vec& input, const vec& target)
		{
			m_trainingData.push_back(std::make_pair(new vec(input), new vec(target)));
		}

		void NetworkTrainer::train(NeuralNetwork& net)
		{
			uint last = m_trainingData.size();
			uint trainingCount = 0;
			float invIterations = 1.f / (float)m_iterations;
			for (uint i = 0; i < m_iterations; i++) {
				uint index = utils::randUint(0, last);
				vec*& input = m_trainingData[index].first;
				vec*& target = m_trainingData[index].second;

				net.computeStep(*input, *target);
				trainingCount++;
				if (trainingCount == m_batchSize) {
					net.takeStep(trainingCount);
					trainingCount = 0;
				}

				if (index != last - 1) {
					std::swap(m_trainingData[index], m_trainingData[last - 1]);
				}
				last--;
				if (last == 0) {
					last = m_trainingData.size();
				}
			}
		}

		NetworkTrainer& NetworkTrainer::operator=(NetworkTrainer rhs)
		{
			cleanup();
			move(rhs);
			return *this;
		}

		void NetworkTrainer::cleanup()
		{
			for (uint i = 0; i < m_trainingData.size(); i++) {
				delete m_trainingData[i].first;
				delete m_trainingData[i].second;
			}
		}

		void NetworkTrainer::move(NetworkTrainer& rhs)
		{
			m_iterations = rhs.m_iterations;
			m_batchSize = rhs.m_batchSize;
			m_trainingData = std::move(rhs.m_trainingData);
		}
	}
}