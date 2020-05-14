#pragma once

#include "..\portaudiowrapper.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		class OutputStream
		{
		private:
			PaStream* m_stream;
			uint m_framesPerBuffer;
			double m_sampleRate;

		public:
			OutputStream(PaDeviceIndex device, uint channelCount, uint framesPerBuffer, double sampleRate);
			OutputStream(uint channelCount, uint framesPerBuffer, double sampleRate);
			~OutputStream();

			void start();
			void stop();

		private:
			static int audioCallback(
				const void* input,
				void* output,
				unsigned long frameCount,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void* userData);
		};
	}
}