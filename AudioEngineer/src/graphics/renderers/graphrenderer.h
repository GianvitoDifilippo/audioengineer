#pragma once

#include "..\data\vertexarray.h"
#include "..\renderables\texture.h"
#include "..\..\core\data\waveform.h"
#include "..\..\core\data\spectrum.h"
#include "..\..\core\data\spectrogram.h"
#include "..\shaders\graphshader.h"
#include "..\shaders\gramshader.h"
#include "..\..\maths\vec4.h"
#include "..\renderables\model.h"
#include "..\data\framebuffer.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class GraphRenderer
		{
		private:
			FrameBuffer* m_waveformFbo;
			FrameBuffer* m_spectrumFbo;
			FrameBuffer* m_spectrogramFbo;
			VertexArray* m_waveformVao;
			VertexArray* m_spectrumVao;
			GraphShader* m_graphShader;
			GramShader* m_gramShader;
			Texture* m_waveformTexture;
			Texture* m_spectrumTexture;
			Texture* m_spectrogramTexture;

			float m_spectrumAmpMax;
			float m_spectrumAmpMaxInv;

		public:
			GraphRenderer();
			~GraphRenderer();

			void setScreenSize(uint width, uint height);

			void render(const core::Waveform& waveform, const maths::vec4& color = maths::vec4(0.8f, 0.f, 0.f, 1.f));
			void render(const core::Spectrum& spectrum, const maths::vec4& color = maths::vec4(0.f, 0.f, 0.8f, 1.f));
			void render(const core::Spectrogram& spectrogram, bool logScale);

			inline Texture* getWaveformTexture() const { return m_waveformTexture; }
			inline Texture* getSpectrumTexture() const { return m_spectrumTexture; }
			inline Texture* getSpectrogramTexture() const { return m_spectrogramTexture; }

			void prepareWaveformAxis(uint size);
			void prepareSpectrumAxis(uint size, bool logScale);

			void setWaveformTextureSize(uint width, uint height);
			void setSpectrumTextureSize(uint width, uint height);
			void setSpectrogramTextureSize(uint width, uint height);
		};
	}
}