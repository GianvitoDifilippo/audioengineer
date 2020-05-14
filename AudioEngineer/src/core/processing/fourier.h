#pragma once

#include "mixfft\mixfft.h"
#include "..\data\waveform.h"
#include "..\data\spectrum.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace core {

		enum WindowType
		{
			RECTANGULAR, TRIANGULAR, WELCH, HAMMING, COSINE, GAUSSIAN
		};

		class Fourier
		{
		protected:
			static fftContext* fc;
			static float *xRe, *xIm, *yRe, *yIm;
			static uint N;
			static float invN;
			static float sqrtN;
			static float invSqrtN;
			bool useCtxt;

			Fourier() { };

		public:
			void useContext(bool useCtxt);
			static void prepareContext(uint length);
			static void cleanUp();
		};

		class DirectTransform : public Fourier
		{
			friend class Fourier;

		private:
			float* window;
			bool useWindow;

			void triangularWindow();
			void welchWindow();
			void hammingWindow();
			void cosineWindow();
			void gaussianWindow(float sigma);

			static DirectTransform m_FFT;

		private:
			DirectTransform();
			~DirectTransform();

		public:
			void setWindow(WindowType window, float param = 0.0f);
			void operator()(const Waveform& waveform, Spectrum& spectrum);
			void operator()(const float* waveform, float* spectrum, size_t size);

			inline static DirectTransform& getFFT() { return m_FFT; }
		};

		class InverseTransform : public Fourier
		{
			friend class Fourier;

			static InverseTransform m_IFFT;

		public:
			InverseTransform();

			void operator()(const Spectrum& spectrum, Waveform& waveform);

			static InverseTransform& getIFFT() { return m_IFFT; }
		};

		extern DirectTransform& FFT;
		extern InverseTransform& IFFT;
	}
}