
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "smallfft.h"
#include "mixfft.h"


/************************************************************************
  fft(int n, float xRe[], float xIm[], float yRe[], float yIm[])
 ------------------------------------------------------------------------
  NOTE : This is copyrighted material, Not public domain. See below.
 ------------------------------------------------------------------------
  Input/output:
      int n          transformation length.
      float xRe[]   real part of input sequence.
      float xIm[]   imaginary part of input sequence.
      float yRe[]   real part of output sequence.
      float yIm[]   imaginary part of output sequence.
 ------------------------------------------------------------------------
  Function:
      The procedure performs a fast discrete Fourier transform (FFT) of
      a complex sequence, x, of an arbitrary length, n. The output, y,
      is also a complex sequence of length n.

      y[k] = sum(x[m]*exp(-i*2*pi*k*m/n), m=0..(n-1)), k=0,...,(n-1)

      The largest prime factor of n must be less than or equal to the
      constant maxPrimeFactor defined below.
 ------------------------------------------------------------------------
  Author:
      Jens Joergen Nielsen            For non-commercial use only.
      Kloevervang 7                   A $100 fee must be paid if used
      DK-2970 Hoersholm               commercially. Please contact.
      DENMARK

      E-mail : jjn2970@gmail.com      All rights reserved. October 2000-2015.
      Homepage : http://home.corix.dk/html/fft.html
 ------------------------------------------------------------------------
  Implementation notes:
      The general idea is to factor the length of the DFT, n, into
      factors that are efficiently handled by the routines.

      A number of short DFT's are implemented with a minimum of
      arithmetical operations and using (almost) straight line code
      resulting in very fast execution when the factors of n belong
      to this set. Especially radix-10 is optimized.

      Prime factors, that are not in the set of short DFT's are handled
      with direct evaluation of the DFP expression.

      Please report any problems to the author. 
      Suggestions and improvements are welcomed.
 ------------------------------------------------------------------------
*************************************************************************/

static float   pi =  3.141592653589793238462643383f;



static void factorize(int n, int *nFact, int fact[])
{
    int i,j,k;
    int nRadix;
    int radices[16];
    int factors[maxFactorCount];

    nRadix    =  7;  
    radices[1]=  2;
    radices[2]=  3;
    radices[3]=  4;
    radices[4]=  5;
    radices[5]=  7;
    radices[6]=  8;
    radices[7]= 10;

    if (n==1) {
        j=1;
        factors[1]=1;
    }
    else j=0;
    i=nRadix;
    while ((n>1) && (i>0)) {
      if ((n % radices[i]) == 0) {
        n=n / radices[i];
        j=j+1;
        factors[j]=radices[i];
      }
      else  i=i-1; 
	}
    if (factors[j] == 2)   /*substitute factors 2*8 with 4*4 */
    {   
      i = j-1;
      while ((i>0) && (factors[i] != 8)) i--;
      if (i>0) {
        factors[j] = 4;
        factors[i] = 4;
      }
    }
    if (n>1) {
        for (k=2; k<sqrt(n)+1; k++)
            while ((n % k) == 0) {
                n=n / k;
                j=j+1;
                factors[j]=k;
            }
        if (n>1) {
            j=j+1;
            factors[j]=n;
        }
    } 

	for (i=1; i<=j; i++)         
    {
      fact[i] = factors[i];
    }
    *nFact=j;
}   /* factorize */

/****************************************************************************
  After N is factored the parameters that control the stages are generated.
  For each stage we have:
    sofar   : the product of the radices so far.
    actual  : the radix handled in this stage.
    remain  : the product of the remaining radices.
 ****************************************************************************/

static void transTableSetup(int sofar[], int actual[], int remain[],
                     int *nFact,
                     int *nPoints)
{
    int i;

    factorize(*nPoints, nFact, actual);
    if (actual[1] > maxPrimeFactor)
    {
        printf("\nPrime factor of FFT length too large : %6d",actual[1]);
        printf("\nPlease modify the value of maxPrimeFactor in mixfft.c");
        exit(1);
    }
    remain[0]=*nPoints;
    sofar[1]=1;
    remain[1]=*nPoints / actual[1];
    for (i=2; i<=*nFact; i++)
    {
        sofar[i]=sofar[i-1]*actual[i-1];
        remain[i]=remain[i-1] / actual[i];
    }
}   /* transTableSetup */

/****************************************************************************
  The sequence y is the permuted input sequence x so that the following
  transformations can be performed in-place, and the final result is the
  normal order.
 ****************************************************************************/

static void permute(int nPoint, int nFact,
             int fact[], int remain[],
             float xRe[], float xIm[],
             float yRe[], float yIm[])

{
    int i,j,k;
    int count[maxFactorCount]; 

    for (i=1; i<=nFact; i++) count[i]=0;
    k=0;
    for (i=0; i<=nPoint-2; i++)
    {
        yRe[i] = xRe[k];
        yIm[i] = xIm[k];
        j=1;
        k=k+remain[j];
        count[1] = count[1]+1;
        while (count[j] >= fact[j])
        {
            count[j]=0;
            k=k-remain[j-1]+remain[j+1];
            j=j+1;
            count[j]=count[j]+1;
        }
    }
    yRe[nPoint-1]=xRe[nPoint-1];
    yIm[nPoint-1]=xIm[nPoint-1];
}   /* permute */

/****************************************************************************
  Same as permute but changes sign on imaginary component. 
  Used in inverse FFT.
 ****************************************************************************/

static void permuteInv(int nPoint, int nFact,
             int fact[], int remain[],
             float xRe[], float xIm[],
             float yRe[], float yIm[])

{
    int i,j,k;
    int count[maxFactorCount]; 

    for (i=1; i<=nFact; i++) count[i]=0;
    k=0;
    for (i=0; i<=nPoint-2; i++)
    {
        yRe[i] = xRe[k];
        yIm[i] = -xIm[k];
        j=1;
        k=k+remain[j];
        count[1] = count[1]+1;
        while (count[j] >= fact[j])
        {
            count[j]=0;
            k=k-remain[j-1]+remain[j+1];
            j=j+1;
            count[j]=count[j]+1;
        }
    }
    yRe[nPoint-1]=xRe[nPoint-1];
    yIm[nPoint-1]=-xIm[nPoint-1];
}   /* permute */

/*Same as above, but takes the input from an array holding both real and imaginary parts
  of the signal in the sequence: IQIQIQ...IQ. Output is in two separate arrays.

  xRe[i] = xIQ[2*i]
  xIm[i] = xIQ[2*i+1]

*/

static void permuteIQ(int nPoint, int nFact,
             int fact[], int remain[],
             float xIQ[],
             float yRe[], float yIm[])

{
    int i,j,k,n;
    int count[maxFactorCount]; 

    for (i=1; i<=nFact; i++) count[i]=0;
    k=0;
    for (i=0; i<=nPoint-2; i++)
    {
		n = k+k;
        yRe[i] = xIQ[n];
        yIm[i] = xIQ[n+1];
        j=1;
        k=k+remain[j];
        count[1] = count[1]+1;
        while (count[j] >= fact[j])
        {
            count[j]=0;
            k=k-remain[j-1]+remain[j+1];
            j=j+1;
            count[j]=count[j]+1;
        }
    }
	n = 2*(nPoint-1);
    yRe[nPoint-1]=xIQ[n];
    yIm[nPoint-1]=xIQ[n+1];
}   /* permute */


/****************************************************************************
  Twiddle factor multiplications and transformations are performed on a
  group of data. The number of multiplications with 1 are reduced by skipping
  the twiddle multiplication of the first stage and of the first group of the
  following stages.
 ***************************************************************************/

static void initTrig(int radix, float trigRe[], float trigIm[])
{
    int i;
    float w, xre, xim;

    w = 2.0f * pi / radix;
    trigRe[0] = 1.0f; trigIm[0] = 0.0f;
    xre = cosf(w); 
    xim = -sinf(w);
    trigRe[1] = xre; trigIm[1] = xim;
    for (i = 2; i < radix; i++)
    {
        trigRe[i] = xre * trigRe[i-1] - xim * trigIm[i-1];
        trigIm[i] = xim * trigRe[i-1] + xre * trigIm[i-1];
    }
}   /* initTrig */



static void initTrigAccurate(int radix, float trigRe[], float trigIm[])
{
    int i;
    float w;

    w = 2.0f * pi / radix;
    for (i = 0; i < radix; i++)
    {
        trigRe[i] =  cosf(i*w);
        trigIm[i] = -sinf(i*w);
    }
}   /* initTrigAccurate */



static void twiddleTransf(int sofarRadix, int radix, int remainRadix, float yRe[], float yIm[])

{   /* twiddleTransf */ 
    float  cosw, sinw, gem;
	int    groupOffset, dataOffset;
	int    dataNo, twNo;
	float  omega, tw_re, tw_im;
    float  trigRe[maxPrimeFactor], trigIm[maxPrimeFactor];
	float  twiddleRe[maxPrimeFactor], twiddleIm[maxPrimeFactor];


    if (radix > 10) initTrig(radix, trigRe, trigIm);
    omega = 2.0f * pi / (float)(sofarRadix * radix);
    cosw =  cosf(omega);
    sinw = -sinf(omega);
    tw_re = 1.0f;
    tw_im = 0.0f;
    dataOffset = 0;
    groupOffset = dataOffset;
    for (dataNo = 0; dataNo < sofarRadix; dataNo++)
    {
		/*calc. twiddlefactors to be used remainRadix times*/

        twiddleRe[0] = 1.0f; 
        twiddleIm[0] = 0.0f;
        twiddleRe[1] = tw_re;
        twiddleIm[1] = tw_im;

        for (twNo = 2; twNo < radix; twNo++) {
            twiddleRe[twNo] = tw_re * twiddleRe[twNo-1] - tw_im * twiddleIm[twNo-1];
            twiddleIm[twNo] = tw_im * twiddleRe[twNo-1] + tw_re * twiddleIm[twNo-1];
        }

        gem   = cosw * tw_re - sinw * tw_im;
        tw_im = sinw * tw_re + cosw * tw_im;
        tw_re = gem;                      
        
		switch(radix) {
			case 2:
		        twiddleFFT_2(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 3:  
		        twiddleFFT_3(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 4:
		        twiddleFFT_4(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 5:  
		        twiddleFFT_5(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 7:
		        twiddleFFT_7(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 8:  
		        twiddleFFT_8(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 10: 
		        twiddleFFT_10(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			default:  
				twiddleFFT_odd(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, trigRe, trigIm, yRe, yIm); 
				break;
		}
		dataOffset++;
        groupOffset = dataOffset;
    }
}   /* twiddleTransf */

static void twiddleTransfCtxt(int sofarRadix, int radix, int remainRadix, float tblRe[], float tblIm[], float yRe[], float yIm[])

{   /* twiddleTransf */ 
	int    groupOffset, dataOffset;
	int    dataNo, twNo;
    float  trigRe[maxPrimeFactor], trigIm[maxPrimeFactor];
	float  twiddleRe[maxPrimeFactor], twiddleIm[maxPrimeFactor];

	int    twiddleIncrement, twiddleIndex;


    if (radix > 10) initTrig(radix, trigRe, trigIm);

	twiddleIncrement = 0;
	dataOffset = 0;
    groupOffset = dataOffset;
    for (dataNo = 0; dataNo < sofarRadix; dataNo++)
    {
		/*calc. twiddlefactors to be used remainRadix times*/
		twiddleIndex = 0;

        for (twNo = 0; twNo < radix; twNo++) {
            twiddleRe[twNo] = tblRe[twiddleIndex];
            twiddleIm[twNo] = tblIm[twiddleIndex];
			twiddleIndex = twiddleIndex + twiddleIncrement;
        }

		twiddleIncrement = twiddleIncrement + remainRadix;

		switch(radix) {
			case 2:
		        twiddleFFT_2(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 3:  
		        twiddleFFT_3(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 4:
		        twiddleFFT_4(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 5:  
		        twiddleFFT_5(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 7:
		        twiddleFFT_7(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 8:  
		        twiddleFFT_8(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			case 10: 
		        twiddleFFT_10(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, yRe, yIm);
				break;
			default:  
				twiddleFFT_odd(sofarRadix, radix, remainRadix, groupOffset, twiddleRe, twiddleIm, trigRe, trigIm, yRe, yIm); 
				break;
		}
		dataOffset++;
        groupOffset = dataOffset;
    }
}   /* twiddleTransfCtxt */

static void transf(int sofarRadix, int radix, int remainRadix, float yRe[], float yIm[])

{   /* Transf */ 
	int    groupOffset,dataOffset;
	int    dataNo;
    float  trigRe[maxPrimeFactor], trigIm[maxPrimeFactor];

    if (radix > 10) initTrig(radix, trigRe, trigIm);
    dataOffset = 0;
    groupOffset = dataOffset;
    for (dataNo = 0; dataNo < sofarRadix; dataNo++)
    {
		switch(radix) {
			case 2:
		        FFT_2(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 3:  
		        FFT_3(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 4:
		        FFT_4(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 5:  
		        FFT_5(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 7:  
		        FFT_7(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 8:  
		        FFT_8(sofarRadix, radix, remainRadix, groupOffset, yRe, yIm);
				break;
			case 10:
		        FFT_10(sofarRadix, radix, remainRadix, groupOffset,yRe, yIm);
				break;
			default:  
				FFT_odd(sofarRadix, radix, remainRadix, groupOffset, trigRe, trigIm, yRe, yIm); 
				break;
		}
		dataOffset++;
        groupOffset = dataOffset;
    }
}   /* transf */

void fft(int n, float xRe[], float xIm[], float yRe[], float yIm[])
{
    int sofarRadix[maxFactorCount], 
        actualRadix[maxFactorCount], 
        remainRadix[maxFactorCount];
    int nFactor;
    int count;

    transTableSetup(sofarRadix, actualRadix, remainRadix, &nFactor, &n);
    permute(n, nFactor, actualRadix, remainRadix, xRe, xIm, yRe, yIm);

    transf(sofarRadix[1], actualRadix[1], remainRadix[1], yRe, yIm);
	if (nFactor > 1)
		for (count = 2; count <= nFactor; count++)
		  twiddleTransf(sofarRadix[count], actualRadix[count], remainRadix[count], yRe, yIm);
}   /* fft */

void fftInv(int n, float xRe[], float xIm[], float yRe[], float yIm[])
{
    int   sofarRadix[maxFactorCount], 
          actualRadix[maxFactorCount], 
          remainRadix[maxFactorCount];
    int   nFactor;
    int   count, i;
	float scaleRe, scaleIm;

    transTableSetup(sofarRadix, actualRadix, remainRadix, &nFactor, &n);
    permuteInv(n, nFactor, actualRadix, remainRadix, xRe, xIm, yRe, yIm);

    transf(sofarRadix[1], actualRadix[1], remainRadix[1], yRe, yIm);
	if (nFactor > 1)
		for (count = 2; count <= nFactor; count++)
		  twiddleTransf(sofarRadix[count], actualRadix[count], remainRadix[count], yRe, yIm); 

	scaleRe = 1.0f / (float) n;
	scaleIm = -scaleRe;

	for (i = 0; i < n; i++) {
		yRe[i] = scaleRe * yRe[i];
		yIm[i] = scaleIm * yIm[i];
	}
}   /* fftInv */


void fftCtxt(struct fftContext *fc, float xRe[], float xIm[], float yRe[], float yIm[])
{
    int count;

    permute(fc->length, fc->factorCount, fc->actualRdx, fc->remainRdx, xRe, xIm, yRe, yIm);

    transf(fc->sofarRdx[1], fc->actualRdx[1], fc->remainRdx[1], yRe, yIm);

	if (fc->factorCount > 1)
		for (count = 2; count <= fc->factorCount; count++)
		  twiddleTransfCtxt(fc->sofarRdx[count], fc->actualRdx[count], fc->remainRdx[count], fc->tableRe, fc->tableIm, yRe, yIm);
}   /* fftCtxt */

void fftInvCtxt(struct fftContext *fc, float xRe[], float xIm[], float yRe[], float yIm[])
{
    int   count, i;
	float scaleRe, scaleIm;

    permuteInv(fc->length, fc->factorCount, fc->actualRdx, fc->remainRdx, xRe, xIm, yRe, yIm);

    transf(fc->sofarRdx[1], fc->actualRdx[1], fc->remainRdx[1], yRe, yIm);

	if (fc->factorCount > 1)
		for (count = 2; count <= fc->factorCount; count++)
		  twiddleTransfCtxt(fc->sofarRdx[count], fc->actualRdx[count], fc->remainRdx[count], fc->tableRe, fc->tableIm, yRe, yIm);
	scaleRe = 1.0f / (float) fc->length;
	scaleIm = -scaleRe;

	for (i = 0; i < fc->length; i++) {
		yRe[i] = scaleRe * yRe[i];
		yIm[i] = scaleIm * yIm[i];
	}
}   /* fftInvCtxt */


void fftCtxtIQ(struct fftContext *fc, float xIQ[], float yRe[], float yIm[])
{
    int count;

    permuteIQ(fc->length, fc->factorCount, fc->actualRdx, fc->remainRdx, xIQ, yRe, yIm);

    transf(fc->sofarRdx[1], fc->actualRdx[1], fc->remainRdx[1], yRe, yIm);

	if (fc->factorCount > 1)
		for (count = 2; count <= fc->factorCount; count++)
		  twiddleTransfCtxt(fc->sofarRdx[count], fc->actualRdx[count], fc->remainRdx[count], fc->tableRe, fc->tableIm, yRe, yIm);
}   /* fftCtxtIQ */


int createFftContext(int fftLength, struct fftContext *fc)
{
	int nFactor, n;

    fc->tableRe = (float *) calloc(fftLength, sizeof(float));                        
    fc->tableIm = (float *) calloc(fftLength, sizeof(float)); 
	
	initTrigAccurate(fftLength, fc->tableRe, fc->tableIm);

    n = fftLength;
	transTableSetup(fc->sofarRdx, fc->actualRdx, fc->remainRdx, &nFactor, &n);
	fc->length = fftLength;
	fc->factorCount = nFactor;

	return 0;
}

int freeFftContext(struct fftContext *fc)
{
    free(fc->tableRe);
    free(fc->tableIm);
	return 0;
}

