#include "joint.h"

namespace audioengineer {
	namespace core {

		Joint::Joint(uint inputCount, uint outputCount)
			: m_weights(outputCount * inputCount), m_biases(outputCount), m_weightsStep(outputCount * inputCount), m_biasesStep(outputCount)
		{
			m_inputCount = inputCount;
			m_outputCount = outputCount;
			
			float wLim = sqrt(2.f / (inputCount + outputCount));
			float bLim = sqrt(1.f / outputCount);
			for (uint i = 0; i < outputCount; i++) {
				for (uint j = 0; j < inputCount; j++) {
					m_weights[i * inputCount + j] = utils::randFloat(-wLim, wLim);
				}
				m_biases[i] = utils::randFloat(-bLim, bLim);
			}
		}

	}
}