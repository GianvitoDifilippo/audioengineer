#include "portaudiowrapper.h"

namespace audioengineer {
	namespace core {

		PaError PortAudio::m_err;
		EventHandler* PortAudio::m_eventHandler;

		void PortAudio::initialize()
		{
			m_eventHandler = EventHandler::getDefaultHandler();
			setError(Pa_Initialize());
		}
		
		void PortAudio::terminate()
		{
			setError(Pa_Terminate());
		}

		void PortAudio::openStream(
			PaStream** stream,
			const PaStreamParameters* inputParameters,
			const PaStreamParameters* outputParameters,
			double sampleRate,
			unsigned long framesPerBuffer,
			PaStreamFlags streamFlags,
			PaStreamCallback* streamCallback,
			void* userData)
		{
			setError(Pa_OpenStream(stream, inputParameters, outputParameters, sampleRate, framesPerBuffer, streamFlags, streamCallback, userData));
		}

		void PortAudio::openDefaultStream(
			PaStream** stream,
			int numInputChannels,
			int numOutputChannels,
			PaSampleFormat sampleFormat,
			double sampleRate,
			unsigned long framesPerBuffer,
			PaStreamCallback* streamCallback,
			void* userData)
		{
			setError(Pa_OpenDefaultStream(stream, numInputChannels, numOutputChannels, sampleFormat, sampleRate, framesPerBuffer, streamCallback, userData));
		}

		void PortAudio::closeStream(PaStream* stream)
		{
			setError(Pa_CloseStream(stream));
		}

		void PortAudio::startStream(PaStream* stream)
		{
			setError(Pa_StartStream(stream));
		}

		void PortAudio::stopStream(PaStream* stream)
		{
			setError(Pa_StopStream(stream));
		}

		std::string PortAudio::getError()
		{
			return std::string(Pa_GetErrorText(m_err));
		}

		void PortAudio::setEventHandler(EventHandler* eventHandler)
		{
			m_eventHandler = eventHandler;
		}

		void PortAudio::setError(PaError err)
		{
			m_err = err;
			if (err != paNoError) {
				m_eventHandler->submitEvent(ErrorEvent("PortAudio error: " + getError()));
			}
		}
	}
}