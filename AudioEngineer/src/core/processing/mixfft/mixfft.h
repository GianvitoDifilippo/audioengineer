#pragma once

#define  maxPrimeFactor        37
#define  maxPrimeFactorDiv2    (maxPrimeFactor+1)/2
#define  maxFactorCount        20

struct fftContext {
	int length;
	int factorCount;
    int sofarRdx[maxFactorCount], 
        actualRdx[maxFactorCount], 
        remainRdx[maxFactorCount];
    float   *tableRe, *tableIm;             
};

/*
The simple fft routine
No preparations before the call. All trig tables created on the fly.
Input: 
	n: the fft length, largest prime factor must be <= maxPrimeFactor
	are, aim: the real and imaginary parts of the input vector
Output:
	bre, bim: the real and imaginary parts of the input vector 
*/
void fft(int n,float are[],float aim[],float bre[],float bim[]);

/*
The simple inverse fft routine
Input: 
	n: the fft length, largest prime factor must be <= maxPrimeFactor
	are, aim: the real and imaginary parts of the input vector
Output:
	bre, bim: the real and imaginary parts of the input vector 
*/
void fftInv(int n,float are[],float aim[],float bre[],float bim[]);

/*
Create and prepare trigonometric and factor tables for the context fft routine
*/
int createFftContext(int fftLength, struct fftContext *fc);

/*
Remove trigonometric and factor tables for the context fft routine
*/
int freeFftContext(struct fftContext *fc);

/*
A table based fft routine
Call the createFftContext with a handle to the table structure before using the routine.
Also remember to call freeFftContext when the table structure is no longer needed.
Input: 
	fftContext*: handle to fft table structure, which includes the fft length
	xre, xim: the real and imaginary parts of the input vector
Output:
	yre, yim: the real and imaginary parts of the input vector 
*/
void fftCtxt(struct fftContext *fc, float xRe[], float xIm[], float yRe[], float yIm[]);

/*
A table based fft routine
Call the createFftContext with a handle to the table structure before using the routine.
Also remember to call freeFftContext when the table structure is no longer needed.
Input: 
	fftContext*: handle to fft table structure, which includes the fft length
	xIQ: the alternating real and imaginary parts of the complex input vector 
Output:
	yre, yim: the real and imaginary parts of the input vector 
*/
void fftCtxtIQ(struct fftContext *fc, float xIQ[], float yRe[], float yIm[]);