#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

#define  u5    1.256637061435917295385057353f   /*  u5   2*pi/5                 */
#define  c5_1 -1.25f                            /*  c5_1 (cos(u5)+cos(2*u5))/2-1*/
#define  c5_2  0.5590169943749474241022934171f  /*  c5_2 (cos(u5)-cos(2*u5))/2  */
#define  c5_3 -0.9510565162951535721164393334f  /*  c5_3 -sin(u5)               */
#define  c5_4 -1.538841768587626701285145288f   /*  c5_4 -(sin(u5)+sin(2*u5))   */
#define  c5_5  0.3632712640026804429477333787f  /*  c5_5 (sin(u5)-sin(2*u5))    */

void dft_5(float aRe[], float aIm[])
{    
    float  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
    float  t4_re,t4_im, t5_re,t5_im;
    float  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
    float  m1_re,m1_im, m5_re,m5_im;
    float  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
    float  s4_re,s4_im, s5_re,s5_im;

    t1_re=aRe[1] + aRe[4]; t1_im=aIm[1] + aIm[4];
    t2_re=aRe[2] + aRe[3]; t2_im=aIm[2] + aIm[3];
    t3_re=aRe[1] - aRe[4]; t3_im=aIm[1] - aIm[4];
    t4_re=aRe[3] - aRe[2]; t4_im=aIm[3] - aIm[2];
    t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
    aRe[0]=aRe[0] + t5_re; aIm[0]=aIm[0] + t5_im;
    m1_re=c5_1*t5_re; m1_im=c5_1*t5_im;
    m2_re=c5_2*(t1_re - t2_re); m2_im=c5_2*(t1_im - t2_im);

    m3_re=-c5_3*(t3_im + t4_im); m3_im=c5_3*(t3_re + t4_re);
    m4_re=-c5_4*t4_im; m4_im=c5_4*t4_re;
    m5_re=-c5_5*t3_im; m5_im=c5_5*t3_re;

    s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
    s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
    s1_re=aRe[0] + m1_re; s1_im=aIm[0] + m1_im;
    s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
    s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;

    aRe[1]=s2_re + s3_re; aIm[1]=s2_im + s3_im;
    aRe[2]=s4_re + s5_re; aIm[2]=s4_im + s5_im;
    aRe[3]=s4_re - s5_re; aIm[3]=s4_im - s5_im;
    aRe[4]=s2_re - s3_re; aIm[4]=s2_im - s3_im;
}   /* dft_5 */



void twiddleFFT_10(int sofarRadix, int radix, int remainRadix, int groupOffset,
				   float twiddleRe[], float twiddleIm[],
				   float yRe[], float yIm[])
{
	int yIndx[10];
	int groupNo, blockNo, adr, i;
    float  aRe[5], aIm[5], bRe[5], bIm[5];
	float  yr, yi, twr, twi;

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
		blockNo = 5;
		for (i = 0; i < 5; i++) {
			yr = yRe[yIndx[blockNo]]; yi = yIm[yIndx[blockNo]];
			twr = twiddleRe[blockNo]; twi = twiddleIm[blockNo];
			bRe[i] =  twr * yr - twi * yi;
			bIm[i] =  twr * yi + twi * yr;
			blockNo = (blockNo + 2) % 10;
		}

		dft_5(aRe, aIm); dft_5(bRe, bIm);

		yRe[yIndx[0]] = aRe[0] + bRe[0]; yRe[yIndx[5]] = aRe[0] - bRe[0];
		yRe[yIndx[6]] = aRe[1] + bRe[1]; yRe[yIndx[1]] = aRe[1] - bRe[1];
		yRe[yIndx[2]] = aRe[2] + bRe[2]; yRe[yIndx[7]] = aRe[2] - bRe[2];
		yRe[yIndx[8]] = aRe[3] + bRe[3]; yRe[yIndx[3]] = aRe[3] - bRe[3];
		yRe[yIndx[4]] = aRe[4] + bRe[4]; yRe[yIndx[9]] = aRe[4] - bRe[4];

		yIm[yIndx[0]] = aIm[0] + bIm[0]; yIm[yIndx[5]] = aIm[0] - bIm[0];
		yIm[yIndx[6]] = aIm[1] + bIm[1]; yIm[yIndx[1]] = aIm[1] - bIm[1];
		yIm[yIndx[2]] = aIm[2] + bIm[2]; yIm[yIndx[7]] = aIm[2] - bIm[2];
		yIm[yIndx[8]] = aIm[3] + bIm[3]; yIm[yIndx[3]] = aIm[3] - bIm[3];
		yIm[yIndx[4]] = aIm[4] + bIm[4]; yIm[yIndx[9]] = aIm[4] - bIm[4];

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}

void FFT_10(int sofarRadix, int radix, int remainRadix, int groupOffset,
				   float yRe[], float yIm[])
{
	int yIndx[10];
	int groupNo, blockNo, adr, i;
    float  aRe[5], aIm[5], bRe[5], bIm[5];

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
			aRe[i] =  yRe[yIndx[blockNo]];
			aIm[i] =  yIm[yIndx[blockNo]];
			i++;
		}

		i = 0;
		blockNo = 5;
		for (i = 0; i < 5; i++) {
			bRe[i] =  yRe[yIndx[blockNo]];
			bIm[i] =  yIm[yIndx[blockNo]];
			blockNo = (blockNo + 2) % 10;
		}

		dft_5(aRe, aIm); dft_5(bRe, bIm);

		yRe[yIndx[0]] = aRe[0] + bRe[0]; yRe[yIndx[5]] = aRe[0] - bRe[0];
		yRe[yIndx[6]] = aRe[1] + bRe[1]; yRe[yIndx[1]] = aRe[1] - bRe[1];
		yRe[yIndx[2]] = aRe[2] + bRe[2]; yRe[yIndx[7]] = aRe[2] - bRe[2];
		yRe[yIndx[8]] = aRe[3] + bRe[3]; yRe[yIndx[3]] = aRe[3] - bRe[3];
		yRe[yIndx[4]] = aRe[4] + bRe[4]; yRe[yIndx[9]] = aRe[4] - bRe[4];

		yIm[yIndx[0]] = aIm[0] + bIm[0]; yIm[yIndx[5]] = aIm[0] - bIm[0];
		yIm[yIndx[6]] = aIm[1] + bIm[1]; yIm[yIndx[1]] = aIm[1] - bIm[1];
		yIm[yIndx[2]] = aIm[2] + bIm[2]; yIm[yIndx[7]] = aIm[2] - bIm[2];
		yIm[yIndx[8]] = aIm[3] + bIm[3]; yIm[yIndx[3]] = aIm[3] - bIm[3];
		yIm[yIndx[4]] = aIm[4] + bIm[4]; yIm[yIndx[9]] = aIm[4] - bIm[4];

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}

