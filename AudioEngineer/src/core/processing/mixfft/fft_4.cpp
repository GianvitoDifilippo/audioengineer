#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

void twiddleFFT_4(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float twiddleRe[], float twiddleIm[],
				  float yRe[], float yIm[])
{
	int yIndx[4];
	int groupNo, blockNo, adr;
    float  t1_re,t1_im, t2_re,t2_im;
    float  m2_re,m2_im, m3_re,m3_im;
	float  yr, yi, twr, twi;
	float  ar0, ar1, ar2, ar3;
	float  ai0, ai1, ai2, ai3;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {
		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		ar0=yRe[yIndx[0]];
		ai0=yIm[yIndx[0]];

		yr = yRe[yIndx[2]]; yi = yIm[yIndx[2]];
		twr = twiddleRe[2]; twi = twiddleIm[2];
		ar2 =  twr * yr - twi * yi;
		ai2 =  twr * yi + twi * yr;

		t1_re = ar0 + ar2; m2_re = ar0 - ar2; 
		t1_im = ai0 + ai2; m2_im = ai0 - ai2;

		yr = yRe[yIndx[1]]; yi = yIm[yIndx[1]];
		twr = twiddleRe[1]; twi = twiddleIm[1];
		ar1 =  twr * yr - twi * yi;
		ai1 =  twr * yi + twi * yr;

		yr = yRe[yIndx[3]]; yi = yIm[yIndx[3]];
		twr = twiddleRe[3]; twi = twiddleIm[3];
		ar3 =  twr * yr - twi * yi;
		ai3 =  twr * yi + twi * yr;

		t2_re = ar1 + ar3; m3_im = ar3 - ar1;
		t2_im = ai1 + ai3; m3_re = ai1 - ai3; 

		yRe[yIndx[0]] = t1_re + t2_re; 
		yRe[yIndx[1]] = m2_re + m3_re; 
		yRe[yIndx[2]] = t1_re - t2_re; 
		yRe[yIndx[3]] = m2_re - m3_re; 
		
		yIm[yIndx[0]] = t1_im + t2_im;
		yIm[yIndx[1]] = m2_im + m3_im;
		yIm[yIndx[2]] = t1_im - t2_im;
		yIm[yIndx[3]] = m2_im - m3_im;

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
	
void FFT_4(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float yRe[], float yIm[])
{
	int yIndx[4];
	int groupNo, blockNo, adr;
    float  t1_re,t1_im, t2_re,t2_im;
    float  m2_re,m2_im, m3_re,m3_im;
	float  ar0, ar1, ar2, ar3;
	float  ai0, ai1, ai2, ai3;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {
		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		ar0=yRe[yIndx[0]];
		ai0=yIm[yIndx[0]];

		ar1=yRe[yIndx[1]];
		ai1=yIm[yIndx[1]];

		ar2=yRe[yIndx[2]];
		ai2=yIm[yIndx[2]];

		ar3=yRe[yIndx[3]];
		ai3=yIm[yIndx[3]];


		t1_re = ar0 + ar2; m2_re = ar0 - ar2; 
		t1_im = ai0 + ai2; m2_im = ai0 - ai2;

		t2_re = ar1 + ar3; m3_im = ar3 - ar1;
		t2_im = ai1 + ai3; m3_re = ai1 - ai3; 

		yRe[yIndx[0]] = t1_re + t2_re; 
		yRe[yIndx[1]] = m2_re + m3_re; 
		yRe[yIndx[2]] = t1_re - t2_re; 
		yRe[yIndx[3]] = m2_re - m3_re; 
		
		yIm[yIndx[0]] = t1_im + t2_im;
		yIm[yIndx[1]] = m2_im + m3_im;
		yIm[yIndx[2]] = t1_im - t2_im;
		yIm[yIndx[3]] = m2_im - m3_im;

		groupOffset=groupOffset+sofarRadix*radix;
		adr=groupOffset;
	}
}
