#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define		NROW	1024
#define		NCOL	NROW

#define TEST_RESULTS


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

//Input Array A
int inputArrayA  [NROW][NCOL];
//Input Array B
int inputArrayB  [NROW][NCOL];
//Weights
int Weight [NROW][NCOL];
//Output Array C
int outputArrayC [NROW][NCOL];


int main(int argc, char* argv[])
{
	double totalSum;

	//INITIALIZE ARRAYS
	for(int i=0;i<NROW;i++)
	{
		for(int j=0;j<NCOL;j++)
		{
			inputArrayA[i][j]= i*NCOL+j;
			inputArrayB[i][j]= j*NCOL+j;
			Weight[i][j]= i*NCOL+j;
			outputArrayC[i][j]= 0;
		}
	}

	//Get the start time
	gettimeofday(&startTime, NULL); /* START TIME */

	//=========================================================

	int tid, nthreads;

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel shared(inputArrayA,inputArrayB,Weight, outputArrayC) private(nthreads, tid)
	{
		/* Obtain thread number */
		tid = omp_get_thread_num();
		nthreads = omp_get_num_threads();

		for(int row=0; row<NROW; row++)
		{
			if (tid == row % nthreads) {
				for(int col=0; col<NCOL; col++){
					int mySum = 0;
					for(int k=0; k<NROW; k++) {
						mySum += inputArrayA[row][k]*Weight[k][col];
					}
					mySum += inputArrayB[row][col]; //only add once
					outputArrayC[row][col] += mySum;
				}
			}
		}

	}


	/* All threads join master thread and disband */

	//=========================================================
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */


	#ifdef TEST_RESULTS
	//CALCULATE TOTAL SUM
	//[Just for verification]
	totalSum=0;
	//
	for(int i=0;i<NROW;i++)
	{
		for(int j=0;j<NCOL;j++)
		{
			totalSum+=(double)outputArrayC[i][j];
		}
	}

	printf("\nTotal Sum = %g\n",totalSum);
	#endif

	//Calculate the interval length
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000
	+ (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval length
	printf("Interval length: %g msec.\n", timeIntervalLength);

	return 0;
}
