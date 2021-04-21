#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define		NROW	1024
#define		NCOL	NROW
#define		NUM_THREADS	4


#define TEST_RESULTS


//Input Array A
int inputArrayA  [NROW][NCOL];
//Input Array B
int inputArrayB  [NROW][NCOL];
//Weights
int Weight [NROW][NCOL];
//Output Array C
int outputArrayC [NROW][NCOL];

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

void *parallelCalc(void *threadid) {
	long tid;
	tid = (long)threadid;

	int chunk_rows = NROW/NUM_THREADS;

	int start_row = chunk_rows*tid;
	int end_row = start_row+chunk_rows;

	for(int row=start_row; row<end_row; row++)
	{
		for(int col=0; col<NCOL; col++)
		{
			int mySum = 0;
			for(int i=0; i<NROW; i++) {
				mySum += inputArrayA[row][i]*Weight[i][col];
			}
			mySum += inputArrayB[row][col]; //only add once
			outputArrayC[row][col] += mySum;
		}
	}
}


int main(int argc, char* argv[])
{
	int i,j,k;
	double totalSum;
	
	

	//INITIALIZE ARRAYS
	for(i=0;i<NROW;i++)
	{
		for(j=0;j<NCOL;j++)
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



	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(nthreads, tid) {
		/* Obtain thread number */
		int tid = omp_get_thread_num();
		int nthreads = omp_get_num_threads();

		
		
		
		
		
		
		
		
		
	}  /* All threads join master thread and disband */

	//=========================================================
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */



	#ifdef TEST_RESULTS
	//CALCULATE TOTAL SUM
	//[Just for verification]
	totalSum=0;
	//
	for(i=0;i<NROW;i++)
	{
		for(j=0;j<NCOL;j++)
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
