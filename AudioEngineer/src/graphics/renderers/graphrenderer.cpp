#include "graphrenderer.h"

namespace audioengineer {
	namespace graphics {

		GraphRenderer::GraphRenderer()
		{
			m_waveformVao = nullptr;
			m_spectrumVao = nullptr;

			m_waveformFbo = new FrameBuffer(256, 256, TEXTURE);
			m_spectrumFbo = new FrameBuffer(256, 256, TEXTURE);
			m_spectrogramFbo = new FrameBuffer(256, 256, TEXTURE);

			m_waveformTexture = new Texture(0, 0, 0);
			m_spectrumTexture = new Texture(0, 0, 0);
			m_spectrogramTexture = new Texture(0, 0, 0);

			m_graphShader = new GraphShader();
			m_gramShader = new GramShader();

			m_spectrumAmpMax = 1.f;
			m_spectrumAmpMaxInv = 1.f;
		}

		GraphRenderer::~GraphRenderer()
		{
			if (m_waveformVao) {
				for (ArrayBuffer* buffer : m_waveformVao->getBuffers()) {
					delete buffer;
				}
			}
			if (m_spectrumVao) {
				for (ArrayBuffer* buffer : m_spectrumVao->getBuffers()) {
					delete buffer;
				}
			}
			delete m_waveformVao;
			delete m_spectrumVao;
			delete m_graphShader;
			delete m_gramShader;
			delete m_waveformFbo;
			delete m_spectrumFbo;
			delete m_spectrogramFbo;
			delete m_waveformTexture;
			delete m_spectrumTexture;
			delete m_spectrogramTexture;
		}

		void GraphRenderer::setScreenSize(uint width, uint height)
		{
			m_waveformFbo->setScreenSize(width, height);
			m_spectrumFbo->setScreenSize(width, height);
			m_spectrogramFbo->setScreenSize(width, height);
		}

		void GraphRenderer::render(const core::Waveform& waveform, const maths::vec4& color)
		{
			float* samples = waveform.getSamples();
			size_t size = waveform.getSize();
			ArrayBuffer* waveformBuffer = m_waveformVao->getBuffers()[1];
			waveformBuffer->bufferData(samples, size, GL_STREAM_DRAW);

			m_waveformFbo->bind();
			glClear(GL_COLOR_BUFFER_BIT);
			m_graphShader->enable();
			m_graphShader->loadColor(color);
			m_waveformVao->enable();
			glLineWidth(1.f);
			glDrawArrays(GL_LINE_STRIP, 0, size);
			m_waveformVao->disable();
			m_graphShader->disable();
			m_waveformFbo->unbind();
		}

		void GraphRenderer::render(const core::Spectrum& spectrum, const maths::vec4& color)
		{
			m_spectrumFbo->bind();
			glClear(GL_COLOR_BUFFER_BIT);
			std::complex<float>* samples = spectrum.getSamples();
			ArrayBuffer* spectrumBuffer = m_spectrumVao->getBuffers()[1];
			size_t size = spectrum.getSize() / 2;
			float* y = new float[size];
			bool overshoot = false;
			for (uint i = 0; i < size; i++) {
				float re = samples[i].real();
				float im = samples[i].imag();
				y[i] = sqrt(re * re + im * im);
				if (y[i] > m_spectrumAmpMax) {
					m_spectrumAmpMax = y[i];
					m_spectrumAmpMaxInv = 1.f / y[i];
					overshoot = true;
				}
			}
			for (uint i = 0; i < size; i++) {
				y[i] = y[i] * m_spectrumAmpMaxInv * 2.f - 1.f;
			}
			if (!overshoot) {
				m_spectrumAmpMax *= 0.9921875f;
				m_spectrumAmpMaxInv = 1.f / m_spectrumAmpMax;
			}
			spectrumBuffer->bufferData(y, size, GL_STREAM_DRAW);

			m_graphShader->enable();
			m_graphShader->loadColor(color);
			m_spectrumVao->enable();
			glLineWidth(1.f);
			glDrawArrays(GL_LINE_STRIP, 0, size);
			m_spectrumVao->disable();
			m_graphShader->disable();
			m_spectrumFbo->unbind();
			delete[] y;
		}

		void GraphRenderer::render(const core::Spectrogram& spectrogram, bool logScale)
		{
			float* samples = spectrogram.getSamples();
			size_t timeSize = spectrogram.getTimeSize();
			size_t freqSize = spectrogram.getFreqSize();
			uint indicesCount = 4 * (timeSize - 1) * (freqSize - 1);
			uint* indices = new uint[indicesCount];
			for (uint i = 0; i < timeSize - 1; i++) {
				for (uint j = 0; j < freqSize - 1; j++) {
					uint index = i * (freqSize - 1) + j;
					uint k = i * freqSize + j;
					indices[4 * index + 0] = k;
					indices[4 * index + 1] = k + freqSize;
					indices[4 * index + 2] = k + freqSize + 1;
					indices[4 * index + 3] = k + 1;
				}
			}
			float min = 20.f * log10(samples[0]);
			float max = 20.f * log10(samples[0]);
			for (uint i = 0; i < timeSize * freqSize; i++) {
				if (samples[i] != 0.f) {
					float sampleLog = 20.f * log10(samples[i]);
					if (sampleLog < min) min = sampleLog;
					if (sampleLog > max) max = sampleLog;
				}
			}
			float M = (float)timeSize;
			float N = (float)freqSize;
			float* xy = new float[timeSize * freqSize * 2];
			float* z = new float[timeSize * freqSize];
			for (uint i = 0; i < timeSize; i++) {
				float x = ((float)i / M) * 2.f - 1.f;
				for (uint j = 0; j < freqSize; j++) {
					float y = 0;
					if (logScale) {
						y = log2(1.f + (N - 2.f) / N * (float)j) / (log2(N) - 1.f) * 2.f - 1.f;
					}
					else {
						y = ((float)j / N) * 2.f - 1.f;
					}
					xy[2 * (i * freqSize + j) + 0] = x;
					xy[2 * (i * freqSize + j) + 1] = y;
					float sample = samples[i * freqSize + j];
					float intensity = 0.f;
					float invRange = 1.f / (max - min);
					if (sample != 0.f) {
						float logSample = 20.f * log10(sample);
						intensity = (logSample - min) * invRange;
					}
					z[i * freqSize + j] = intensity * intensity;
				}
			}
			IndexBuffer indexBuffer(indices, indicesCount);
			VertexArray spectrogramVao;
			ArrayBuffer axes(2);
			ArrayBuffer values(1);
			spectrogramVao.vertexAttribPointer(axes);
			spectrogramVao.vertexAttribPointer(values);
			spectrogramVao.setIndexBuffer(indexBuffer);
			axes.bufferData(xy, timeSize * freqSize * 2, GL_STREAM_DRAW);
			values.bufferData(z, timeSize * freqSize, GL_STREAM_DRAW);
			delete[] xy;
			delete[] z;
			delete[] indices;
			
			m_spectrogramFbo->bind();
			m_gramShader->enable();
			spectrogramVao.enable();
			glDrawElements(GL_QUADS, indicesCount, GL_UNSIGNED_INT, nullptr);
			spectrogramVao.disable();
			m_gramShader->disable();
			m_spectrogramFbo->unbind();
		}

		void GraphRenderer::prepareWaveformAxis(uint size)
		{
			if (m_waveformVao) {
				for (ArrayBuffer* buffer : m_waveformVao->getBuffers()) {
					delete buffer;
				}
			}
			delete m_waveformVao;
			m_waveformVao = new VertexArray();
			ArrayBuffer* timeAxis = new ArrayBuffer(1);
			ArrayBuffer* waveform = new ArrayBuffer(1);
			m_waveformVao->vertexAttribPointer(*timeAxis);
			m_waveformVao->vertexAttribPointer(*waveform);
			std::vector<float> axis(size);
			for (uint i = 0; i < size; i++) {
				axis[i] = ((float)i / (float)size) * 2.f - 1.f;
			}
			timeAxis->bufferData(axis, GL_STATIC_DRAW);
		}

		void GraphRenderer::prepareSpectrumAxis(uint size, bool logScale)
		{
			if (m_spectrumVao) {
				for (ArrayBuffer* buffer : m_spectrumVao->getBuffers()) {
					delete buffer;
				}
			}
			delete m_spectrumVao;
			m_spectrumVao = new VertexArray();
			ArrayBuffer* freqAxis = new ArrayBuffer(1);
			ArrayBuffer* spectrum = new ArrayBuffer(1);
			m_spectrumVao->vertexAttribPointer(*freqAxis);
			m_spectrumVao->vertexAttribPointer(*spectrum);
			float N = (float)size;
			std::vector<float> axis(size / 2);
			for (uint i = 0; i < size / 2; i++) {
				if (logScale) {
					axis[i] = log2(1.f + (N - 2.f) / N * (float)i) / (log2(N) - 1.f) * 2.f - 1.f;
				}
				else {
					axis[i] = ((float)i / N * 2.f) * 2.f - 1.f;
				}
			}
			freqAxis->bufferData(axis, GL_STATIC_DRAW);
		}

		void GraphRenderer::setWaveformTextureSize(uint width, uint height)
		{
			m_waveformFbo->setSize(width, height);
			m_waveformTexture->setID(m_waveformFbo->getColorID());
			m_waveformTexture->setWidth(m_waveformFbo->getWidth());
			m_waveformTexture->setHeight(m_waveformFbo->getHeight());
		}

		void GraphRenderer::setSpectrumTextureSize(uint width, uint height)
		{
			m_spectrumFbo->setSize(width, height);
			m_spectrumTexture->setID(m_spectrumFbo->getColorID());
			m_spectrumTexture->setWidth(m_spectrumFbo->getWidth());
			m_spectrumTexture->setHeight(m_spectrumFbo->getHeight());
		}

		void GraphRenderer::setSpectrogramTextureSize(uint width, uint height)
		{
			m_spectrogramFbo->setSize(width, height);
			m_spectrogramTexture->setID(m_spectrogramFbo->getColorID());
			m_spectrogramTexture->setWidth(m_spectrogramFbo->getWidth());
			m_spectrogramTexture->setHeight(m_spectrogramFbo->getHeight());
		}
	}
}