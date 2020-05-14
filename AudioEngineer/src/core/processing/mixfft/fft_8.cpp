#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

#define  c8      0.7071067811865475244008443620f   /*  c8  1/sqrt(2)     */
	
void dft_4(float aRe[], float aIm[])
{
    float  t1_re, t1_im, t2_re, t2_im;
    float  m2_re, m2_im, m3_re, m3_im;
	
	{
		float  ax, ay;

		ax = aRe[0]; ay = aRe[2];
		t1_re = ax + ay; 
		m2_re = ax - ay; 

		ax = aIm[0]; ay = aIm[2];
		t1_im = ax + ay;
		m2_im = ax - ay;
    
		ax = aRe[1]; ay = aRe[3];
		t2_re = ax + ay; 
		m3_im = ay - ax;

		ax = aIm[1]; ay = aIm[3];
		t2_im = ax + ay;
		m3_re = ax - ay; 
	}

    aRe[0] = t1_re + t2_re; 
	aRe[1] = m2_re + m3_re; 
    aRe[2] = t1_re - t2_re; 
    aRe[3] = m2_re - m3_re; 
		   	 
	aIm[0] = t1_im + t2_im;
	aIm[1] = m2_im + m3_im;
	aIm[2] = t1_im - t2_im;
	aIm[3] = m2_im - m3_im;
}   /* dft_4 */


void twiddleFFT_8(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float twiddleRe[], float twiddleIm[],
				  float yRe[], float yIm[])
{
	int yIndx[8];
	int groupNo, blockNo, adr, i;
    float aRe[4], aIm[4], bRe[4], bIm[4], gem;
	float yr, yi, twr, twi;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {

		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		aRe[0] = yRe[yIndx[0]];
		aIm[0] = yIm[yIndx[0]];

		i = 1;
		for (blockNo = 2; blockNo < radix; blockNo += 2) {
			yr = yRe[yIndx[blockNo]]; yi = yIm[yIndx[blockNo]];
			twr = twiddleRe[blockNo]; twi = twiddleIm[blockNo];
			aRe[i] =  twr * yr - twi * yi;
			aIm[i] =  twr * yi + twi * yr;
			i++;
		}

		i = 0;
		for (blockNo = 1; blockNo < radix; blockNo += 2) {
			yr = yRe[yIndx[blockNo]]; yi = yIm[yIndx[blockNo]];
			twr = twiddleRe[blockNo]; twi = twiddleIm[blockNo];
			bRe[i] =  twr * yr - twi * yi;
			bIm[i] =  twr * yi + twi * yr;
			i++;
		}

		dft_4(aRe, aIm); dft_4(bRe, bIm);

		gem    =  c8 * (bRe[1] + bIm[1]);
		bIm[1] =  c8 * (bIm[1] - bRe[1]);
		bRe[1] =  gem;
		gem    =  bIm[2];
		bIm[2] = -bRe[2];
		bRe[2] =  gem;
		gem    =  c8 * (bIm[3] - bRe[3]);
		bIm[3] = -c8 * (bRe[3] + bIm[3]);
		bRe[3] =  gem;

		yRe[yIndx[0]] = aRe[0] + bRe[0]; yRe[yIndx[4]] = aRe[0] - bRe[0];
		yRe[yIndx[1]] = aRe[1] + bRe[1]; yRe[yIndx[5]] = aRe[1] - bRe[1];
		yRe[yIndx[2]] = aRe[2] + bRe[2]; yRe[yIndx[6]] = aRe[2] - bRe[2];
		yRe[yIndx[3]] = aRe[3] + bRe[3]; yRe[yIndx[7]] = aRe[3] - bRe[3];

		yIm[yIndx[0]] = aIm[0] + bIm[0]; yIm[yIndx[4]] = aIm[0] - bIm[0];
		yIm[yIndx[1]] = aIm[1] + bIm[1]; yIm[yIndx[5]] = aIm[1] - bIm[1];
		yIm[yIndx[2]] = aIm[2] + bIm[2]; yIm[yIndx[6]] = aIm[2] - bIm[2];
		yIm[yIndx[3]] = aIm[3] + bIm[3]; yIm[yIndx[7]] = aIm[3] - bIm[3];

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
void FFT_8(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float yRe[], float yIm[])
{
	int yIndx[8];
	int groupNo, blockNo, adr, i;
    float  aRe[4], aIm[4], bRe[4], bIm[4], gem;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {

		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		i = 0;
		for (blockNo = 0; blockNo < radix; blockNo += 2) {
			aRe[i] =  yRe[yIndx[blockNo]];
			aIm[i] =  yIm[yIndx[blockNo]];
			i++;
		}

		i = 0;
		for (blockNo = 1; blockNo < radix; blockNo += 2) {
			bRe[i] = yRe[yIndx[blockNo]];
			bIm[i] = yIm[yIndx[blockNo]];
			i++;
		}

		dft_4(aRe, aIm); dft_4(bRe, bIm);

		gem    =  c8 * (bRe[1] + bIm[1]);
		bIm[1] =  c8 * (bIm[1] - bRe[1]);
		bRe[1] =  gem;
		gem    =  bIm[2];
		bIm[2] = -bRe[2];
		bRe[2] =  gem;
		gem    =  c8 * (bIm[3] - bRe[3]);
		bIm[3] = -c8 * (bRe[3] + bIm[3]);
		bRe[3] =  gem;

		yRe[yIndx[0]] = aRe[0] + bRe[0]; yRe[yIndx[4]] = aRe[0] - bRe[0];
		yRe[yIndx[1]] = aRe[1] + bRe[1]; yRe[yIndx[5]] = aRe[1] - bRe[1];
		yRe[yIndx[2]] = aRe[2] + bRe[2]; yRe[yIndx[6]] = aRe[2] - bRe[2];
		yRe[yIndx[3]] = aRe[3] + bRe[3]; yRe[yIndx[7]] = aRe[3] - bRe[3];

		yIm[yIndx[0]] = aIm[0] + bIm[0]; yIm[yIndx[4]] = aIm[0] - bIm[0];
		yIm[yIndx[1]] = aIm[1] + bIm[1]; yIm[yIndx[5]] = aIm[1] - bIm[1];
		yIm[yIndx[2]] = aIm[2] + bIm[2]; yIm[yIndx[6]] = aIm[2] - bIm[2];
		yIm[yIndx[3]] = aIm[3] + bIm[3]; yIm[yIndx[7]] = aIm[3] - bIm[3];

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
