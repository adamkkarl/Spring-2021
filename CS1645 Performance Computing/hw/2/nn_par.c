#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>


#define		NROW	1024
#define		NCOL	NROW
#define NUM_THREADS	8
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


void *parallelCalc(void *threadid) {
	long tid;
	tid = (long)threadid;

	int chunk_rows = NROW/NUM_THREADS;

	//Y = RELU(AVE(XW))
	for (int i=0; i<NROW; i+=NUM_THREADS)
	{
		for(int j=0; j<NCOL; j++)
		{
			for(int k=0; k<NCOL; k++)
			{
				outputY[i]+=inputArrayX[i][k]*Weight[k][j];
				// printf("%g\n",outputY[i]);
			}
			outputY[i] /= NCOL;
		}
		if (outputY[i] < 0){outputY[i] = 0;}
	}


}

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

	// DECLARE THREADS
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	void *status;


	// CREATE MULTIPLE THREADS
	for(t=0; t<NUM_THREADS; t++){
		//printf("In main: creating thread %ld\n", t);

		rc = pthread_create(&threads[t], NULL, parallelCalc, (void *)t);

		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// JOIN THREADS
	for(t=0; t<NUM_THREADS; t++) {
		rc = pthread_join(threads[t], &status);
	}

	//Y = SOFTMAX(Y)
	//Loss = Ground - Y
	for (int i =0; i < NROW; i++){
		outputY[i] = exp(outputY[i]);
		total_sum += outputY[i];
	}
	//
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
