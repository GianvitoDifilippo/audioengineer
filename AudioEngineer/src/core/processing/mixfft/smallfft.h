#pragma once

void twiddleFFT_2(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_2(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_4(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_4(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_8(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_8(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_3(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_3(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_5(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_5(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_7(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float yRe[], float yIm[]);

void FFT_7(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float yRe[], float yIm[]);

void twiddleFFT_10(int sofarRadix, int radix, int remainRadix, int groupOffset,
		   float twiddleRe[], float twiddleIm[],
		   float yRe[], float yIm[]);

void FFT_10(int sofarRadix, int radix, int remainRadix, int groupOffset,
		   float yRe[], float yIm[]);

void twiddleFFT_odd(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float twiddleRe[], float twiddleIm[],
		  float trigRe[], float trigIm[],
		  float yRe[], float yIm[]);

void FFT_odd(int sofarRadix, int radix, int remainRadix, int groupOffset,
		  float trigRe[], float trigIm[],
		  float yRe[], float yIm[]);
