#include "outputstream.h"

namespace audioengineer {
	namespace core {

		OutputStream::OutputStream(PaDeviceIndex device, uint channelCount, uint framesPerBuffer, double sampleRate)
		{
			m_framesPerBuffer = framesPerBuffer;
			m_sampleRate = sampleRate;
			PaStreamParameters outputParameters;
			outputParameters.channelCount = channelCount;
			outputParameters.device = device;
			outputParameters.hostApiSpecificStreamInfo = nullptr;
			outputParameters.sampleFormat = paFloat32;
			outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowOutputLatency;
			PortAudio::openStream(&m_stream, nullptr, &outputParameters, sampleRate, framesPerBuffer, paClipOff, audioCallback, this);
		}

		OutputStream::OutputStream(uint channelCount, uint framesPerBuffer, double sampleRate)
		{
			m_framesPerBuffer = framesPerBuffer;
			m_sampleRate = sampleRate;
			PortAudio::openDefaultStream(&m_stream, 0, channelCount, paFloat32, sampleRate, framesPerBuffer, audioCallback, this);
		}

		OutputStream::~OutputStream()
		{
			PortAudio::closeStream(m_stream);
		}

		void OutputStream::start()
		{
			PortAudio::startStream(m_stream);
		}

		void OutputStream::stop()
		{
			PortAudio::stopStream(m_stream);
		}

		int OutputStream::audioCallback(
			const void* input,
			void* output,
			unsigned long frameCount,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void* userData)
		{
			float* out = static_cast<float*>(output);
			OutputStream* ostream = static_cast<OutputStream*>(userData);
			(void)input;
			(void)timeInfo;
			(void)statusFlags;

			for (uint i = 0; i < frameCount; i++) {
				
			}

			return paContinue;
		}
	}
}