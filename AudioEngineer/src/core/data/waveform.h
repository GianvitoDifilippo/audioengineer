#pragma once

#include <iostream>
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		class Waveform
		{
		private:
			float* m_samples;
			double m_sampleRate;
			size_t m_size;
			uint m_samplePointer;

		public:
			Waveform(double sampleRate, size_t size);
			Waveform(const Waveform& rhs);
			Waveform(Waveform&& rhs);
			~Waveform();

			void clear();
			void setSamplePointer(uint samplePointer);
			bool pushBack(float sample);
			float& operator[](uint index);
			Waveform& operator++();
			Waveform& operator--();

			inline float* getSamples() const { return m_samples; }
			inline double getSampleRate() const { return m_sampleRate; }
			inline uint getSamplePointer() const { return m_samplePointer; }
			inline size_t getSize() const { return m_size; }

			Waveform& operator=(Waveform rhs);

		private:
			void cleanup();
			void move(Waveform& rhs);
		};
	}
}