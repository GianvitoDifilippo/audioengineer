#pragma once

#include <portaudio.h>
#include <string>
#include "..\events\eventsystem.h"
#include "..\defines.h"

namespace audioengineer {
	namespace core {

		class PortAudio
		{
		private:
			static PaError m_err;
			static EventHandler* m_eventHandler;

		public:
			static void initialize();
			static void terminate();
			static void openStream(
				PaStream** stream,
				const PaStreamParameters* inputParameters,
				const PaStreamParameters* outputParameters,
				double sampleRate,
				unsigned long framesPerBuffer,
				PaStreamFlags streamFlags,
				PaStreamCallback* streamCallback,
				void* userData);
			static void openDefaultStream(
				PaStream** stream,
				int numInputChannels,
				int numOutputChannels,
				PaSampleFormat sampleFormat,
				double sampleRate,
				unsigned long framesPerBuffer,
				PaStreamCallback* streamCallback,
				void* userData);
			static void closeStream(PaStream* stream);
			static void startStream(PaStream* stream);
			static void stopStream(PaStream* stream);

			static void setEventHandler(EventHandler* eventHandler);

			static std::string getError();
			inline static EventHandler* getEventHandler() { return m_eventHandler; }

		private:
			PortAudio(PaError err);

			static void setError(PaError err);
		};
	}
}