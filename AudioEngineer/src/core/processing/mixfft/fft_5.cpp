#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

#define  u5    1.256637061435917295385057353f  /*  u5   2*pi/5                 */
#define  c5_1 -1.25f                           /*  c5_1 (cos(u5)+cos(2*u5))/2-1*/
#define  c5_2  0.5590169943749474241022934171f /*  c5_2 (cos(u5)-cos(2*u5))/2  */
#define  c5_3 -0.9510565162951535721164393334f /*  c5_3 -sin(u5)               */
#define  c5_4 -1.538841768587626701285145288f  /*  c5_4 -(sin(u5)+sin(2*u5))   */
#define  c5_5  0.3632712640026804429477333787f /*  c5_5 (sin(u5)-sin(2*u5))    */

void twiddleFFT_5(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float twiddleRe[], float twiddleIm[],
				  float yRe[], float yIm[])
{
	int yIndx[5];
	int groupNo, blockNo, adr;
    float  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
    float  t4_re,t4_im, t5_re,t5_im;
    float  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
    float  m1_re,m1_im, m5_re,m5_im;
    float  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
    float  s4_re,s4_im, s5_re,s5_im;

	float  yr, yi, twr, twi;
	float  ar0, ar1, ar2, ar3, ar4;
	float  ai0, ai1, ai2, ai3, ai4;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {
		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		ar0=yRe[yIndx[0]];
		ai0=yIm[yIndx[0]];

		yr = yRe[yIndx[1]]; yi = yIm[yIndx[1]];
		twr = twiddleRe[1]; twi = twiddleIm[1];
		ar1 =  twr * yr - twi * yi;
		ai1 =  twr * yi + twi * yr;

		yr = yRe[yIndx[2]]; yi = yIm[yIndx[2]];
		twr = twiddleRe[2]; twi = twiddleIm[2];
		ar2 =  twr * yr - twi * yi;
		ai2 =  twr * yi + twi * yr;

		yr = yRe[yIndx[3]]; yi = yIm[yIndx[3]];
		twr = twiddleRe[3]; twi = twiddleIm[3];
		ar3 =  twr * yr - twi * yi;
		ai3 =  twr * yi + twi * yr;

		yr = yRe[yIndx[4]]; yi = yIm[yIndx[4]];
		twr = twiddleRe[4]; twi = twiddleIm[4];
		ar4 =  twr * yr - twi * yi;
		ai4 =  twr * yi + twi * yr;

		t1_re = ar1 + ar4; t1_im = ai1 + ai4;
		t2_re = ar2 + ar3; t2_im = ai2 + ai3;
		t3_re = ar1 - ar4; t3_im = ai1 - ai4;
		t4_re = ar3 - ar2; t4_im = ai3 - ai2;
		t5_re = t1_re + t2_re; t5_im = t1_im + t2_im;
		ar0 = ar0 + t5_re; ai0 = ai0 + t5_im;
		m1_re = c5_1*t5_re; m1_im = c5_1*t5_im;
		m2_re = c5_2*(t1_re - t2_re); m2_im = c5_2*(t1_im - t2_im);

		m3_re = -c5_3*(t3_im + t4_im); m3_im = c5_3*(t3_re + t4_re);
		m4_re = -c5_4*t4_im; m4_im = c5_4*t4_re;
		m5_re = -c5_5*t3_im; m5_im = c5_5*t3_re;

		s3_re = m3_re - m4_re; s3_im = m3_im - m4_im;
		s5_re = m3_re + m5_re; s5_im = m3_im + m5_im;
		s1_re = ar0 + m1_re; s1_im = ai0 + m1_im;
		s2_re = s1_re + m2_re; s2_im = s1_im + m2_im;
		s4_re = s1_re - m2_re; s4_im = s1_im - m2_im;

		yRe[yIndx[0]] = ar0;  
		yIm[yIndx[0]] = ai0;
		yRe[yIndx[1]] = s2_re + s3_re; yIm[yIndx[1]] = s2_im + s3_im;
		yRe[yIndx[2]] = s4_re + s5_re; yIm[yIndx[2]] = s4_im + s5_im;
		yRe[yIndx[3]] = s4_re - s5_re; yIm[yIndx[3]] = s4_im - s5_im;
		yRe[yIndx[4]] = s2_re - s3_re; yIm[yIndx[4]] = s2_im - s3_im;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
void FFT_5(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float yRe[], float yIm[])
{
	int yIndx[5];
	int groupNo, blockNo, adr;
    float  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
    float  t4_re,t4_im, t5_re,t5_im;
    float  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
    float  m1_re,m1_im, m5_re,m5_im;
    float  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
    float  s4_re,s4_im, s5_re,s5_im;

	float  ar0, ar1, ar2, ar3, ar4;
	float  ai0, ai1, ai2, ai3, ai4;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {
		for (blockNo = 0; blockNo < radix; blockNo++) {
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

		ar4=yRe[yIndx[4]];
		ai4=yIm[yIndx[4]];

		t1_re = ar1 + ar4; t1_im = ai1 + ai4;
		t2_re = ar2 + ar3; t2_im = ai2 + ai3;
		t3_re = ar1 - ar4; t3_im = ai1 - ai4;
		t4_re = ar3 - ar2; t4_im = ai3 - ai2;
		t5_re = t1_re + t2_re; t5_im = t1_im + t2_im;
		ar0 = ar0 + t5_re; ai0 = ai0 + t5_im;
		m1_re = c5_1*t5_re; m1_im = c5_1*t5_im;
		m2_re = c5_2*(t1_re - t2_re); m2_im = c5_2*(t1_im - t2_im);

		m3_re = -c5_3*(t3_im + t4_im); m3_im = c5_3*(t3_re + t4_re);
		m4_re = -c5_4*t4_im; m4_im = c5_4*t4_re;
		m5_re = -c5_5*t3_im; m5_im = c5_5*t3_re;

		s3_re = m3_re - m4_re; s3_im = m3_im - m4_im;
		s5_re = m3_re + m5_re; s5_im = m3_im + m5_im;
		s1_re = ar0 + m1_re; s1_im = ai0 + m1_im;
		s2_re = s1_re + m2_re; s2_im = s1_im + m2_im;
		s4_re = s1_re - m2_re; s4_im = s1_im - m2_im;

		yRe[yIndx[0]] = ar0;  
		yIm[yIndx[0]] = ai0;
		yRe[yIndx[1]] = s2_re + s3_re; yIm[yIndx[1]] = s2_im + s3_im;
		yRe[yIndx[2]] = s4_re + s5_re; yIm[yIndx[2]] = s4_im + s5_im;
		yRe[yIndx[3]] = s4_re - s5_re; yIm[yIndx[3]] = s4_im - s5_im;
		yRe[yIndx[4]] = s2_re - s3_re; yIm[yIndx[4]] = s2_im - s3_im;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
