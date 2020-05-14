#include "inputstream.h"

namespace audioengineer {
	namespace core {

		InputStream::InputStream(PaDeviceIndex device, uint channelCount, uint framesPerBuffer, double sampleRate, uint bufferSize)
			: m_buffer(adapt(bufferSize, framesPerBuffer))
		{
			m_bufferIndex = 0;
			m_framesPerBuffer = framesPerBuffer;
			m_sampleRate = sampleRate;
			m_inputParameters.channelCount = channelCount;
			if (device != -1) {
				m_inputParameters.device = device;
				m_inputParameters.hostApiSpecificStreamInfo = nullptr;
				m_inputParameters.sampleFormat = paFloat32;
				m_inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;
				PortAudio::openStream(&m_stream, &m_inputParameters, nullptr, sampleRate, framesPerBuffer, paClipOff, audioCallback, this);
			}
			else {
				PortAudio::openDefaultStream(&m_stream, channelCount, 0, paFloat32, sampleRate, framesPerBuffer, audioCallback, this);
			}
			m_eventHandler = EventHandler::getDefaultHandler();
		}

		InputStream::~InputStream()
		{
			PortAudio::closeStream(m_stream);
		}

		void InputStream::start()
		{
			PortAudio::startStream(m_stream);
		}

		void InputStream::stop()
		{
			PortAudio::stopStream(m_stream);
		}

		void InputStream::streamToWaveform(Waveform& waveform, uint channel)
		{
			if (channel > (uint)m_inputParameters.channelCount) channel = 0;
			uint waveformSize = waveform.getSize();
			uint bufferSize = m_buffer.size();
			uint channelCount = m_inputParameters.channelCount;
			if (waveformSize > bufferSize / channelCount / 2) {
				waveformSize = bufferSize / channelCount / 2;
			}
			uint index = m_bufferIndex + bufferSize - waveformSize * channelCount + channel;
			for (uint i = 0; i < waveformSize; i++) {
				if (index >= bufferSize)
					index -= bufferSize;
				waveform[i] = m_buffer[index];
				index += channelCount;
			}
		}

		void InputStream::recordToWaveform(Waveform& waveform, uint channel)
		{
			if (channel >(uint)m_inputParameters.channelCount) channel = 0;
			m_recordingWaveforms.push_back(&waveform);
			m_recordingChannels.push_back(channel);
		}

		int InputStream::audioCallback(
			const void* input,
			void* output,
			unsigned long frameCount,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void* userData)
		{
			const float* in = static_cast<const float*>(input);
			InputStream* istream = static_cast<InputStream*>(userData);
			uint& bufferIndex = istream->m_bufferIndex;
			std::vector<float>& buffer = istream->m_buffer;
			uint bufferSize = buffer.size();
			std::vector<Waveform*>& recordingWaveforms = istream->m_recordingWaveforms;
			std::vector<uint>& recordingChannels = istream->m_recordingChannels;
			EventHandler* eventHandler = istream->m_eventHandler;
			(void)output;
			(void)timeInfo;
			(void)statusFlags;

			uint channelCount = istream->m_inputParameters.channelCount;

			for (uint i = 0; i < frameCount; i++) {
				for (uint j = 0; j < channelCount; j++) {
					float sample = *in++;
					buffer[bufferIndex++] = sample;
					for (uint k = 0; k < recordingChannels.size(); k++) {
						Waveform& waveform = *recordingWaveforms[k];
						if (j == recordingChannels[k]) {
							if (waveform.pushBack(sample)) {
								recordingWaveforms.erase(std::remove(recordingWaveforms.begin(), recordingWaveforms.end(), &waveform), recordingWaveforms.end());
								recordingChannels.erase(std::remove(recordingChannels.begin(), recordingChannels.end(), recordingChannels[k]), recordingChannels.end());
								eventHandler->submitEvent(AudioEvent(WAVEFORM_RECORDED));
							}
						}
					}
				}
			}
			if (bufferIndex == bufferSize) {
				bufferIndex = 0;
			}

			return paContinue;
		}

		uint InputStream::adapt(uint bufferSize, uint framesPerBuffer)
		{
			if (bufferSize == -1) return 8 * framesPerBuffer;
			return framesPerBuffer * (bufferSize / framesPerBuffer + ((bufferSize % framesPerBuffer) != 0));
		}
	}
}