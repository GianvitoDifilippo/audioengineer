#include "fourier.h"

#define AE_PI	3.1415926535897932384626433832795f
#define AE_PI_2 1.5707963267948966192313216916398f
#define AE_PI_4 0.78539816339744830961566084581988f
#define AE_2PI	6.283185307179586476925286766559f
#define AE_4PI	12.566370614359172953850573533118f

namespace audioengineer {
	namespace core {

		DirectTransform DirectTransform::m_FFT;
		InverseTransform InverseTransform::m_IFFT;

		DirectTransform& FFT = DirectTransform::getFFT();
		InverseTransform& IFFT = InverseTransform::getIFFT();

		fftContext* Fourier::fc;
		float* Fourier::xRe;
		float* Fourier::xIm;
		float* Fourier::yRe;
		float* Fourier::yIm;
		uint Fourier::N;
		float Fourier::invN;
		float Fourier::sqrtN;
		float Fourier::invSqrtN;

		void Fourier::prepareContext(uint length)
		{
			if (fc != NULL) freeFftContext(fc);
			else fc = new fftContext();
			createFftContext(length, fc);
			N = length;
			invN = 1.f / (float)N;
			sqrtN = sqrt((float)N);
			invSqrtN = 1.0f / sqrtN;
			delete[] xRe;
			delete[] xIm;
			delete[] yRe;
			delete[] yIm;
			xRe = new float[N];
			xIm = new float[N];
			yRe = new float[N];
			yIm = new float[N];
			for (uint i = 0; i < N; i++) {
				xIm[i] = 0.f;
			}
		}

		void Fourier::useContext(bool useCtxt)
		{
			this->useCtxt = useCtxt;
		}

		void Fourier::cleanUp()
		{
			if (fc != NULL) {
				freeFftContext(fc);
				fc = NULL;
			}
			delete[] xRe;
			delete[] xIm;
			delete[] yRe;
			delete[] yIm;
		}



		DirectTransform::DirectTransform() { }

		DirectTransform::~DirectTransform()
		{
			delete[] window;
		}

		void DirectTransform::triangularWindow()
		{
			useWindow = true;
			float Nminus1 = (float)(N - 1);
			for (uint n = 0; n < N; n++) {
				window[n] = 1.0f - abs(((float)(n << 1) - Nminus1) / (float)N);
			}
		}

		void DirectTransform::welchWindow()
		{
			useWindow = true;
			float Nminus1 = (float)(N - 1);
			for (uint n = 0; n < N; n++) {
				float p = ((float)(n << 1) - Nminus1) / Nminus1;
				window[n] = 1.0f - p * p;
			}
		}

		void DirectTransform::hammingWindow()
		{
			useWindow = true;
			float Nminus1 = (float)(N - 1);
			for (uint n = 0; n < N; n++) {
				window[n] = 0.54f - 0.46f * cosf(AE_2PI * (float)n / Nminus1);
			}
		}

		void DirectTransform::cosineWindow()
		{
			useWindow = true;
			float Nminus1 = (float)(N - 1);
			for (uint n = 0; n < N; n++) {
				window[n] = sinf((float)n * AE_PI / Nminus1);
			}
		}

		void DirectTransform::gaussianWindow(float sigma)
		{
			useWindow = true;
			float Nminus1 = (float)(N - 1);
			for (uint n = 0; n < N; n++) {
				float exponent = ((float)(n << 1) - Nminus1) / (sigma * Nminus1);
				exponent *= exponent / 2;
				window[n] = expf(-exponent);
			}
		}

		void DirectTransform::setWindow(WindowType window, float param)
		{
			delete[] this->window;
			if(window != RECTANGULAR) this->window = new float[N];
			switch (window) {
			case RECTANGULAR: useWindow = false; break;
			case TRIANGULAR: triangularWindow(); break;
			case WELCH: welchWindow(); break;
			case HAMMING: hammingWindow(); break;
			case COSINE: cosineWindow(); break;
			case GAUSSIAN: gaussianWindow(param); break;
			}
		}

		void DirectTransform::operator()(const Waveform& waveform, Spectrum& spectrum)
		{
			float* samples = waveform.getSamples();
			uint size = waveform.getSize();
			if (size == 0) return;
			if (size != N) prepareContext(size);
			if (useCtxt) {
				if (useWindow) {
					for (uint i = 0; i < N; i++) {
						xRe[i] = samples[i] * window[i];
					}
					fftCtxt(fc, xRe, xIm, yRe, yIm);
				}
				else {
					for (uint i = 0; i < N; i++) {
						xRe[i] = samples[i];
					}
					fftCtxt(fc, xRe, xIm, yRe, yIm);
				}
			}
			else {
				if (useWindow) {
					for (uint i = 0; i < N; i++) {
						xRe[i] = samples[i] * window[i];
					}
					fft(N, xRe, xIm, yRe, yIm);
				}
				else {
					for (uint i = 0; i < N; i++) {
						xRe[i] = samples[i];
					}
					fft(N, xRe, xIm, yRe, yIm);
				}
			}
			for (uint i = 0; i < N / 2; i++) {
				spectrum[i] = std::complex<float>(yRe[i], yIm[i]);
			}
		}

		void DirectTransform::operator()(const float* waveform, float* spectrum, size_t size)
		{
			if (size == 0) return;
			if (size != N) prepareContext(size);
			if (useCtxt) {
				if (useWindow) {
					for (uint i = 0; i < N; i++) xRe[i] = waveform[i] * window[i];
					fftCtxt(fc, xRe, xIm, yRe, yIm);
				}
				else {
					for (uint i = 0; i < N; i++) xRe[i] = waveform[i];
					fftCtxt(fc, xRe, xIm, yRe, yIm);
				}
			}
			else {
				if (useWindow) {
					for (uint i = 0; i < N; i++) xRe[i] = waveform[i] * window[i];
					fft(N, xRe, xIm, yRe, yIm);
				}
				else {
					for (uint i = 0; i < N; i++) xRe[i] = waveform[i];
					fft(N, xRe, xIm, yRe, yIm);
				}
			}
			for (uint i = 0; i < N / 2; i++) {
				spectrum[i] = yRe[i] * yRe[i] + yIm[i] * yIm[i];
			}
		}
#if 0
		void DirectTransform::operator()(const Waveform& waveform, Spectrogram& spectrogram, uint timeIndex)
		{
			float* samples = waveform.getSamples();
			uint size = waveform.getSize();
			if (timeIndex == -1) {
				for (uint i = 0; i < size; i++) {
					(*this)(waveform, spectrogram, i);
				}
			}
			else {
				uint timeSize = spectrogram.getTimeSize();
				uint freqSize = spectrogram.getFreqSize();
				uint windowSize = freqSize * 2;
				Waveform windowWaveform(waveform.getSampleRate(), windowSize);
				for (uint i = 0; i < windowSize; i++) {
					uint index = size + timeIndex + i - freqSize;
					if (index < size || index >= 2 * size) {
						windowWaveform[i] = 0.0f;
					}
					else {
						windowWaveform[i] = samples[index - size];
					}
				}
				Spectrum spectrum(waveform.getSampleRate(), freqSize);
				(*this)(windowWaveform, spectrum);
				for (uint i = 0; i < freqSize; i++) {
					const std::complex<float>& sample = spectrum[i];
					spectrogram(timeIndex, i) = sqrt(sample.real() * sample.real() + sample.imag() * sample.imag());
				}
			}
		}
#endif

		InverseTransform::InverseTransform() {  }

		void InverseTransform::operator()(const Spectrum& spectrum, Waveform& waveform)
		{
			std::complex<float>* samples = spectrum.getSamples();
			uint size = spectrum.getSize();
			if (size == 0) return;
			if (size * 2 != N) prepareContext(size * 2);
			for (uint i = 0; i < size; i++) {
				xRe[i] = samples[i].real();
				xIm[i] = -samples[i].imag();
			}
			for (uint i = 0; i < size; i++) {
				xRe[i + size] = samples[size - 1 - i].real();
				xIm[i + size] = -samples[size - 1 - i].imag();
			}
			if (useCtxt) fftCtxt(fc, xRe, xIm, yRe, yIm);
			else fft(N, xRe, xIm, yRe, yIm);
			for (uint i = 0; i < N; i++) {
				waveform[i] = yIm[i];
			}
		}
	}
}