#pragma once

#include "..\..\defines.h"
#include "..\..\utils\rand.h"

namespace audioengineer {

	namespace utils {
		class FileUtils;
	}

	namespace core {

		class Joint
		{
			friend class audioengineer::utils::FileUtils;

		private:
			uint m_inputCount;
			uint m_outputCount;
			vec m_weights;
			vec m_biases;
			vec m_weightsStep;
			vec m_biasesStep;

		public:
			Joint() = default;
			Joint(uint inputCount, uint outputCount);

			uint getInputCount() const;
			uint getOutputCount() const;
			vec& getWeights();
			vec& getBiases();
			vec& getWeightsStep();
			vec& getBiasesStep();
		};

		inline uint Joint::getInputCount() const
		{
			return m_inputCount;
		}

		inline uint Joint::getOutputCount() const
		{
			return m_outputCount;
		}

		inline vec& Joint::getWeights()
		{
			return m_weights;
		}

		inline vec& Joint::getBiases()
		{
			return m_biases;
		}

		inline vec& Joint::getWeightsStep()
		{
			return m_weightsStep;
		}

		inline vec& Joint::getBiasesStep()
		{
			return m_biasesStep;
		}
	}
}