#include "spectrum.h"

namespace audioengineer {
	namespace core {

		Spectrum::Spectrum(double sampleRate, size_t size)
		{
			m_sampleRate = sampleRate;
			m_size = size;
			m_samples = new std::complex<float>[size];
		}

		Spectrum::Spectrum(const Spectrum& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_size = rhs.m_size;
			m_samples = new std::complex<float>[m_size];
			for (uint i = 0; i < m_size; i++) {
				m_samples[i] = rhs.m_samples[i];
			}
		}

		Spectrum::Spectrum(Spectrum&& rhs)
		{
			move(rhs);
		}

		Spectrum::~Spectrum()
		{
			cleanup();
		}

		std::vector<float> Spectrum::getMagnitude() const
		{
			std::vector<float> result(m_size / 2);
			for (uint i = 0; i < m_size / 2; i++) {
				result[i] = std::abs(m_samples[i]);
			}
			return result;
		}

		std::complex<float>& Spectrum::operator[](uint index)
		{
			if (index >= m_size) {
				index %= m_size;
			}
			return m_samples[index];
		}

		Spectrum& Spectrum::operator=(Spectrum rhs)
		{
			cleanup();
			move(rhs);
			return *this;
		}

		void Spectrum::cleanup()
		{
			delete[] m_samples;
		}

		void Spectrum::move(Spectrum& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_size = rhs.m_size;
			m_samples = rhs.m_samples;
			
			rhs.m_samples = nullptr;
		}
	}
}