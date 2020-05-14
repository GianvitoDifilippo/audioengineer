#include "spectrogram.h"

namespace audioengineer {
	namespace core {

		Spectrogram::Spectrogram(const Waveform& waveform, uint windowSize, uint noverlap)
		{
			if (noverlap == -1) noverlap = windowSize / 2;
			uint N = waveform.getSize();
			m_timeSize = (uint)ceil((float)(N - windowSize) / (float)(windowSize - noverlap) + 1);
			m_freqSize = windowSize / 2;
			m_sampleRate = waveform.getSampleRate() / (double)(windowSize - noverlap);
			m_samplePointer = 0;
			m_noverlap = noverlap;

			m_samples = new float[m_timeSize * m_freqSize];
			float* ptrWaveform = waveform.getSamples();
			float* ptrSpectrum = m_samples;
			for (uint i = 0; i < m_timeSize; i++) {
				FFT(ptrWaveform, ptrSpectrum, windowSize);
				ptrWaveform += windowSize - noverlap;
				ptrSpectrum += m_freqSize;
			}
		}

		Spectrogram::Spectrogram(const Spectrogram& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_timeSize = rhs.m_timeSize;
			m_freqSize = rhs.m_freqSize;
			m_samples = new float[m_timeSize * m_freqSize];
			for (uint i = 0; i < m_timeSize * m_freqSize; i++) {
				m_samples[i] = rhs.m_samples[i];
			}
			m_samplePointer = rhs.m_samplePointer;
			m_noverlap = rhs.m_noverlap;
		}

		Spectrogram::Spectrogram(Spectrogram&& rhs)
		{
			move(rhs);
		}

		Spectrogram::~Spectrogram()
		{
			cleanup();
		}

		float* Spectrogram::operator[](uint timeIndex) const
		{
			if (timeIndex >= m_timeSize) {
				timeIndex %= m_timeSize;
			}
			return m_samples + timeIndex * m_freqSize;
		}

		float& Spectrogram::operator()(uint timeIndex, uint freqIndex)
		{
			if (timeIndex >= m_timeSize) {
				timeIndex %= m_timeSize;
			}
			if (freqIndex >= m_freqSize) {
				freqIndex %= m_freqSize;
			}
			return m_samples[timeIndex * m_freqSize + freqIndex];
		}

		Spectrogram& Spectrogram::operator++()
		{
			m_samplePointer++;
			return *this;
		}

		Spectrogram& Spectrogram::operator--()
		{
			m_samplePointer--;
			return *this;
		}

		Spectrogram& Spectrogram::operator=(Spectrogram rhs)
		{
			cleanup();
			move(rhs);
			return *this;
		}

		void Spectrogram::cleanup()
		{
			delete[] m_samples;
		}

		void Spectrogram::move(Spectrogram& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_timeSize = rhs.m_timeSize;
			m_freqSize = rhs.m_freqSize;
			m_samples = rhs.m_samples;
			m_samplePointer = rhs.m_samplePointer;
			m_noverlap = rhs.m_noverlap;

			rhs.m_samples = nullptr;
		}
	}
}