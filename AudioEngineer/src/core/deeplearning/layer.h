#pragma once

#include "..\..\defines.h"

namespace audioengineer {

	namespace utils {
		class FileUtils;
	}

	namespace core {

		class Layer
		{
		public:
			friend class audioengineer::utils::FileUtils;

		private:
			vec m_neurons;
			std::vector<vec> m_derivativesW;
			std::vector<vec> m_derivativesB;

		public:
			Layer() = default;
			Layer(const std::vector<uint>& topology, uint index);

			uint getNeuronCount() const;
			vec& getNeurons();
			vec& getDerivativesW(uint k);
			vec& getDerivativesB(uint k);
		};

		inline uint Layer::getNeuronCount() const
		{
			return m_neurons.size();
		}

		inline vec& Layer::getNeurons()
		{
			return m_neurons;
		}

		inline vec& Layer::getDerivativesW(uint k)
		{
#ifdef _DEBUG
			if (k >= m_derivativesW.size()) throw std::out_of_range("Layer exception: k >= derivativesW size");
#endif
			return m_derivativesW[k];
		}

		inline vec& Layer::getDerivativesB(uint k)
		{
#ifdef _DEBUG
			if (k >= m_derivativesB.size()) throw std::out_of_range("Layer exception: k >= derivativesB size");
#endif
			return m_derivativesB[k];
		}
	}
}