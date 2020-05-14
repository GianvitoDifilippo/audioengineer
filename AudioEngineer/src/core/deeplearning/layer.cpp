#include "layer.h"

namespace audioengineer {
	namespace core {

		Layer::Layer(const std::vector<uint>& topology, uint index)
			: m_neurons(topology[index])
		{
			uint neuronCount = topology[index];
			m_derivativesW.reserve(index);
			m_derivativesB.reserve(index);
			for (uint k = 0; k < index; k++) {
				m_derivativesW.emplace_back(neuronCount * topology[k] * topology[k + 1]);
				m_derivativesB.emplace_back(neuronCount * topology[k + 1]);
			}
		}

	}
}