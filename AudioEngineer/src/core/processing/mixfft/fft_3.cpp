#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

#define  c3_1  -1.5f                             /*  c3_1  cos(2*pi/3)-1           */
#define  c3_2   0.8660254037844386467637231707f  /*  c3_2  sin(2*pi/3)             */
                                          

void twiddleFFT_3(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float twiddleRe[], float twiddleIm[],
				  float yRe[], float yIm[])
{
	int yIndx[3];
	int groupNo, blockNo, adr;
	float t1_re, t1_im, m1_re, m1_im, m2_re, m2_im, s1_re, s1_im;
	float yr, yi, twr, twi;
	float  ar0, ar1, ar2;
	float  ai0, ai1, ai2;
	float  zr0, zi0;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {
		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}
		ar0 = yRe[yIndx[0]];
		ai0 = yIm[yIndx[0]];

		yr = yRe[yIndx[1]];
		yi = yIm[yIndx[1]];
		twr = twiddleRe[1];
		twi = twiddleIm[1];
		ar1=  twr * yr - twi * yi;
		ai1=  twr * yi + twi * yr;

		yr = yRe[yIndx[2]];
		yi = yIm[yIndx[2]];
		twr = twiddleRe[2];
		twi = twiddleIm[2];
		ar2=  twr * yr - twi * yi;
		ai2=  twr * yi + twi * yr;

		t1_re = ar1 + ar2; 
		t1_im = ai1 + ai2;
		zr0 = ar0 + t1_re; 
		zi0 = ai0 + t1_im;
		m1_re = c3_1*t1_re; 
		m1_im = c3_1*t1_im;
		m2_re = c3_2*(ai1 - ai2); 
		m2_im = c3_2*(ar2 -  ar1);

		s1_re = zr0 + m1_re; 
		s1_im = zi0 + m1_im;

		yRe[yIndx[0]] = zr0; 
		yIm[yIndx[0]] = zi0; 
		yRe[yIndx[1]] = s1_re + m2_re; 
		yIm[yIndx[1]] = s1_im + m2_im;
		yRe[yIndx[2]] = s1_re - m2_re; 
		yIm[yIndx[2]] = s1_im - m2_im;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
void FFT_3(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float yRe[], float yIm[])
{
	int yIndx[3];
	int groupNo, blockNo, adr;
	float  t1_re, t1_im, m1_re, m1_im, m2_re, m2_im, s1_re, s1_im;
	float  ar0, ar1, ar2;
	float  ai0, ai1, ai2;
	float  zr0, zi0;


	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {
		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}
		ar0 = yRe[yIndx[0]];
		ai0 = yIm[yIndx[0]];

		ar1 = yRe[yIndx[1]];
		ai1 = yIm[yIndx[1]];

		ar2 = yRe[yIndx[2]];
		ai2 = yIm[yIndx[2]];

		t1_re = ar1 + ar2; 
		t1_im = ai1 + ai2;
		zr0 = ar0 + t1_re; 
		zi0 = ai0 + t1_im;
		m1_re = c3_1*t1_re; 
		m1_im = c3_1*t1_im;
		m2_re = c3_2*(ai1 - ai2); 
		m2_im = c3_2*(ar2 -  ar1);

		s1_re = zr0 + m1_re; 
		s1_im = zi0 + m1_im;

		yRe[yIndx[0]] = zr0; 
		yIm[yIndx[0]] = zi0; 
		yRe[yIndx[1]] = s1_re + m2_re; 
		yIm[yIndx[1]] = s1_im + m2_im;
		yRe[yIndx[2]] = s1_re - m2_re; 
		yIm[yIndx[2]] = s1_im - m2_im;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
