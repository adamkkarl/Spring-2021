#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>


#define		NROW	1024
#define		NCOL	NROW
#define TEST_RESULTS

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

//Input Array X
double inputArrayX  [NROW][NCOL];
//Weights
double Weight [NROW][NCOL];
//Final output from SOFTMAX
double outputY [NROW];
//Total Sum
double total_sum;
//Ground Truth
double Ground [NROW];
//Loss
double Loss [NROW];
//Check
double val_sum;


int main(int argc, char* argv[])
{
	int i,j,k;
	//INITIALIZE ARRAYS
	for(i=0;i<NROW;i++)
	{
		outputY[i] = 0;
		Ground[i] = 1;
		for(j=0;j<NCOL;j++)
		{
			inputArrayX[i][j]= 0.5;
			Weight[i][j]= 0.4;
		}
	}
	total_sum = 0;

	//Get the start time
	gettimeofday(&startTime, NULL); /* START TIME */
	//=====================================================================


	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel shared(inputArrayX,Weight, outputArrayY, outputY, total_sum)
	{
		/* Obtain thread number */
		int tid = omp_get_thread_num();
		int nthreads = omp_get_num_threads();

		//Y = RELU(AVE(XW))
		for (int i=0; i<NROW; i+=NUM_THREADS)
		{
			if(tid == i % nthreads) {
				for(int j=0; j<NCOL; j++)
				{
					double mysum = 0;
					for(int k=0; k<NCOL; k++)
					{
						mysum+=inputArrayX[i][k]*Weight[k][j];
					}
					outputY[i] = mysum;
					outputY[i] /= NCOL;
				}
				if (outputY[i] < 0){outputY[i] = 0;}
			}
		}

	
		//also parallelized
		//Y = SOFTMAX(Y)
		//Loss = Ground - Y
		for (int i =0; i < NROW; i++){
			if(tid == i % nthreads) {
				outputY[i] = exp(outputY[i]);
				total_sum += outputY[i];
			}
		}
	}
	
	for (int i =0; i < NROW; i++){
		outputY[i] /= total_sum;
		Loss[i] = outputY[i] - Ground[i];
	}

	//=====================================================================
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* after time */

	#ifdef TEST_RESULTS
		//CALCULATE TOTAL SUM
		//[Just for verification]
		val_sum=0;
		//
		for(i=0;i<NROW;i++)
		{
				val_sum+=(double)Loss[i];
		}

		printf("\nTotal Sum = %g\n",val_sum);
	#endif

	//Calculate the interval length
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000
	                     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval lenght
	printf("Interval length: %g msec.\n", timeIntervalLength);


	return 0;
}
