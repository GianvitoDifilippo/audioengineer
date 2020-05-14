#pragma once

#include "waveform.h"
#include <cmath>
#include "..\processing\fourier.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		class Spectrogram
		{
		private:
			float* m_samples;
			double m_sampleRate;
			size_t m_timeSize;
			size_t m_freqSize;
			uint m_samplePointer;
			uint m_noverlap;

		public:
			Spectrogram(const Waveform& waveform, uint windowSize, uint noverlap = -1);
			Spectrogram(const Spectrogram& rhs);
			Spectrogram(Spectrogram&& rhs);
			~Spectrogram();

			float* operator[](uint timeIndex) const;
			float& operator()(uint timeIndex, uint freqIndex);
			Spectrogram& operator++();
			Spectrogram& operator--();

			inline float* getSamples() const { return m_samples; }
			inline double getSampleRate() const { return m_sampleRate; }
			inline size_t getTimeSize() const { return m_timeSize; }
			inline size_t getFreqSize() const { return m_freqSize; }
			inline uint getSamplePointer() const { return m_samplePointer; }

			Spectrogram& operator=(Spectrogram rhs);

		private:
			void cleanup();
			void move(Spectrogram& rhs);
		};
	}
}