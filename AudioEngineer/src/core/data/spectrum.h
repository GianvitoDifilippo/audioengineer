#pragma once

#include <vector>
#include <complex>
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		class Spectrum
		{
		private:
			std::complex<float>* m_samples;
			double m_sampleRate;
			size_t m_size;

		public:
			Spectrum(double sampleRate, size_t size);
			Spectrum(const Spectrum& rhs);
			Spectrum(Spectrum&& rhs);
			~Spectrum();

			std::complex<float>& operator[](uint index);

			inline std::complex<float>* getSamples() const { return m_samples; }
			std::vector<float> getMagnitude() const;
			inline double getSampleRate() const { return m_sampleRate; }
			inline size_t getSize() const { return m_size; }

			Spectrum& operator=(Spectrum rhs);

		private:
			void cleanup();
			void move(Spectrum& rhs);
		};
	}
}