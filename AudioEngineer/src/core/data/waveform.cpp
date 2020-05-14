#include "waveform.h"

namespace audioengineer {
	namespace core {

		Waveform::Waveform(double sampleRate, size_t size)
		{
			m_sampleRate = sampleRate;
			m_size = size;
			m_samples = new float[size];
			m_samplePointer = 0;
		}

		Waveform::Waveform(const Waveform& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_size = rhs.m_size;
			m_samples = new float[m_size];
			for (uint i = 0; i < m_size; i++) {
				m_samples[i] = rhs.m_samples[i];
			}
			m_samplePointer = 0;
		}

		Waveform::Waveform(Waveform&& rhs)
		{
			move(rhs);
		}

		Waveform::~Waveform()
		{
			cleanup();
		}

		void Waveform::clear()
		{
			for (uint i = 0; i < m_size; i++) {
				m_samples[i] = 0.f;
			}
		}

		bool Waveform::pushBack(float sample)
		{
			m_samples[m_samplePointer] = sample;
			operator++();
			return m_samplePointer == 0;
		}

		void Waveform::setSamplePointer(uint samplePointer)
		{
			m_samplePointer = samplePointer % m_size;
		}

		float& Waveform::operator[](uint index)
		{
			if (index >= m_size) {
				index %= m_size;
			}
			return m_samples[index];
		}

		Waveform& Waveform::operator++()
		{
			if (m_samplePointer == m_size - 1) {
				m_samplePointer = 0;
			}
			else {
				m_samplePointer++;
			}
			return *this;
		}

		Waveform& Waveform::operator--()
		{
			if (m_samplePointer == 0) {
				m_samplePointer = m_size - 1;
			}
			else {
				m_samplePointer--;
			}
			return *this;
		}

		Waveform& Waveform::operator=(Waveform rhs)
		{
			cleanup();
			move(rhs);
			return *this;
		}

		void Waveform::cleanup()
		{
			delete[] m_samples;
		}

		void Waveform::move(Waveform& rhs)
		{
			m_sampleRate = rhs.m_sampleRate;
			m_size = rhs.m_size;
			m_samples = rhs.m_samples;
			m_samplePointer = 0;

			rhs.m_samples = nullptr;
		}
	}
}