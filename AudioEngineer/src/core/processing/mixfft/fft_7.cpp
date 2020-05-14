#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"

#define  c7_1  0.222520933956314404288902564496794759466355569f
#define  c7_2  0.900968867902419126236102319507445051165919162f
#define  c7_3  0.623489801858733530525004884004239810632274731f
#define  c7_4  0.433883739117558120475768332848358754609990728f
#define  c7_5  0.781831482468029808708444526674057750232334519f
#define  c7_6  0.974927912181823607018131682993931217232785801f


void twiddleFFT_7(int sofarRadix, int radix, int remainRadix, int groupOffset,
				  float twiddleRe[], float twiddleIm[],
				  float yRe[], float yIm[])
{
	int yIndx[7];
	int groupNo, blockNo, adr, i;
	float  yr, yi, twr, twi;

	float are0, aim0, aRe[7], aIm[7];
	float are16add, are61sub, aim16sub, aim16add, are25add, are52sub, aim25sub, aim25add;
	float are34add, are43sub, aim34sub, aim34add;
	float ax, ay;

	adr = groupOffset;
	for (groupNo = 0; groupNo < remainRadix; groupNo++) {

		for (blockNo = 0; blockNo < radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		aRe[0] = yRe[yIndx[0]];
		aIm[0] = yIm[yIndx[0]];

		i = 1;
		for (blockNo = 1; blockNo < radix; blockNo += 1) {
			yr = yRe[yIndx[blockNo]]; yi = yIm[yIndx[blockNo]];
			twr = twiddleRe[blockNo]; twi = twiddleIm[blockNo];
			aRe[blockNo] = twr * yr - twi * yi;
			aIm[blockNo] = twr * yi + twi * yr;
		}

		are0 = aRe[0];
		aim0 = aIm[0];

		ax = aRe[1]; ay = aRe[6];
		are16add = ax + ay; are61sub = ay - ax;
		ax = aIm[1]; ay = aIm[6];
		aim16sub = ax - ay; aim16add = ax + ay;

		ax = aRe[2]; ay = aRe[5];
		are25add = ax + ay; are52sub = ay - ax;
		ax = aIm[2]; ay = aIm[5];
		aim25sub = ax - ay; aim25add = ax + ay;

		ax = aRe[3]; ay = aRe[4];
		are34add = ax + ay; are43sub = ay - ax;
		ax = aIm[3]; ay = aIm[4];
		aim34sub = ax - ay;	aim34add = ax + ay;

		ax = c7_6 * aim16sub - c7_5 * aim34sub - c7_4 * aim25sub;
		ay = c7_3 * are34add - c7_2 * are25add - c7_1 * are16add + are0;
		yRe[yIndx[5]] = ay - ax; yRe[yIndx[2]] = ay + ax;
		ax = c7_6 * are61sub - c7_5 * are43sub - c7_4 * are52sub;
		ay = c7_3 * aim34add - c7_2 * aim25add - c7_1 * aim16add + aim0;
		yIm[yIndx[2]] = ax + ay; yIm[yIndx[5]] = ay - ax;
		
		ax = c7_5 * aim16sub + c7_6 * aim25sub + c7_4 * aim34sub;
		ay = c7_3 * are16add - c7_1 * are25add - c7_2 * are34add + are0;
		yRe[yIndx[6]] = ay - ax; yRe[yIndx[1]] = ay + ax;
		ax = c7_5 * are61sub + c7_6 * are52sub + c7_4 * are43sub;
		ay = c7_3 * aim16add - c7_1 * aim25add - c7_2 * aim34add + aim0;
		yIm[yIndx[1]] = ax + ay; yIm[yIndx[6]] = ay - ax;

		ax = c7_4 * aim16sub + c7_6 * aim34sub - c7_5 * aim25sub;
		ay = c7_3 * are25add - c7_1 * are34add - c7_2 * are16add + are0;
		yRe[yIndx[4]] = ay - ax; yRe[yIndx[3]] = ay + ax;
		ax = c7_4 * are61sub + c7_6 * are43sub - c7_5 * are52sub;
		ay = c7_3 * aim25add - c7_1 * aim34add - c7_2 * aim16add + aim0;
		yIm[yIndx[3]] = ax + ay; yIm[yIndx[4]] = ay - ax;

		yRe[yIndx[0]] = are0 + are16add + are25add + are34add;
		yIm[yIndx[0]] = aim0 + aim16add + aim25add + aim34add;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
void FFT_7(int sofarRadix, int radix, int remainRadix, int groupOffset, float yRe[], float yIm[])
{
	int yIndx[7];
	int groupNo, blockNo, adr, i;

	float are0, aim0, aRe[7], aIm[7];
	float are16add, are61sub, aim16sub, aim16add, are25add, are52sub, aim25sub, aim25add;
	float are34add, are43sub, aim34sub, aim34add;
	float ax, ay;

	adr = groupOffset;
	for (groupNo=0; groupNo<remainRadix; groupNo++) {

		for (blockNo=0; blockNo<radix; blockNo++) {
			yIndx[blockNo] = adr;
			adr = adr + sofarRadix;
		}

		are0 = yRe[yIndx[0]];
		aim0 = yIm[yIndx[0]];

		i = 1;
		for (blockNo=1; blockNo<radix; blockNo += 1) {
			aRe[blockNo] =  yRe[yIndx[blockNo]];
			aIm[blockNo] =  yIm[yIndx[blockNo]];
		}

		ax = aRe[1]; ay = aRe[6];
		are16add = ax + ay; are61sub = ay - ax;
		ax = aIm[1]; ay = aIm[6];
		aim16sub = ax - ay; aim16add = ax + ay;

		ax = aRe[2]; ay = aRe[5];
		are25add = ax + ay; are52sub = ay - ax;
		ax = aIm[2]; ay = aIm[5];
		aim25sub = ax - ay; aim25add = ax + ay;

		ax = aRe[3]; ay = aRe[4];
		are34add = ax + ay; are43sub = ay - ax;
		ax = aIm[3]; ay = aIm[4];
		aim34sub = ax - ay;	aim34add = ax + ay;

		ax = c7_6 * aim16sub - c7_5 * aim34sub - c7_4 * aim25sub;
		ay = c7_3 * are34add - c7_2 * are25add - c7_1 * are16add + are0;
		yRe[yIndx[5]] = ay - ax; yRe[yIndx[2]] = ay + ax;
		ax = c7_6 * are61sub - c7_5 * are43sub - c7_4 * are52sub;
		ay = c7_3 * aim34add - c7_2 * aim25add - c7_1 * aim16add + aim0;
		yIm[yIndx[2]] = ax + ay; yIm[yIndx[5]] = ay - ax;
		
		ax = c7_5 * aim16sub + c7_6 * aim25sub + c7_4 * aim34sub;
		ay = c7_3 * are16add - c7_1 * are25add - c7_2 * are34add + are0;
		yRe[yIndx[6]] = ay - ax; yRe[yIndx[1]] = ay + ax;
		ax = c7_5 * are61sub + c7_6 * are52sub + c7_4 * are43sub;
		ay = c7_3 * aim16add - c7_1 * aim25add - c7_2 * aim34add + aim0;
		yIm[yIndx[1]] = ax + ay; yIm[yIndx[6]] = ay - ax;

		ax = c7_4 * aim16sub + c7_6 * aim34sub - c7_5 * aim25sub;
		ay = c7_3 * are25add - c7_1 * are34add - c7_2 * are16add + are0;
		yRe[yIndx[4]] = ay - ax; yRe[yIndx[3]] = ay + ax;
		ax = c7_4 * are61sub + c7_6 * are43sub - c7_5 * are52sub;
		ay = c7_3 * aim25add - c7_1 * aim34add - c7_2 * aim16add + aim0;
		yIm[yIndx[3]] = ax + ay; yIm[yIndx[4]] = ay - ax;

		yRe[yIndx[0]] = are0 + are16add + are25add + are34add;
		yIm[yIndx[0]] = aim0 + aim16add + aim25add + aim34add;

		groupOffset = groupOffset + sofarRadix * radix;
		adr = groupOffset;
	}
}
	
