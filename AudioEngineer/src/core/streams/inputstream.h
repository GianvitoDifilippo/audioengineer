#pragma once

#include <algorithm>
#include "..\portaudiowrapper.h"
#include "..\data\waveform.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		class InputStream
		{
		private:
			PaStream* m_stream;
			PaStreamParameters m_inputParameters;
			uint m_framesPerBuffer;
			double m_sampleRate;
			std::vector<float> m_buffer;
			uint m_bufferIndex;
			std::vector<Waveform*> m_recordingWaveforms;
			std::vector<uint> m_recordingChannels;
			EventHandler* m_eventHandler;

		public:
			InputStream(PaDeviceIndex device, uint channelCount, uint framesPerBuffer, double sampleRate, uint bufferSize = -1);
			~InputStream();

			void start();
			void stop();

			void streamToWaveform(Waveform& waveform, uint channel = 0);
			void recordToWaveform(Waveform& waveform, uint channel = 0);

		private:
			static int audioCallback(
				const void* input,
				void* output,
				unsigned long frameCount,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void* userData);

			static uint adapt(uint bufferSize, uint framesPerBuffer);
		};
	}
}