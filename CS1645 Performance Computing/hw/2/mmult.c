#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define		NROW	1024
#define		NCOL	NROW
#define		NUM_THREADS	1


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




	// DECLARE THREADS
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	void *status;

	// CREATE MULTIPLE THREADS
	for(t=0; t<NUM_THREADS; t++){
		//printf("In main: creating thread %ld\n", t);

		rc = pthread_create(&threads[t], NULL, NULL, (void *)t);



		//W*A + B
		for(i=0;i<NROW;i++)
		{
			for(j=0; j<NCOL; j++)
			{
				for(k=0;k<NROW;k++)
				{
					outputArrayC[i][j]+=inputArrayA[i][k]*Weight[k][j];
				}
				outputArrayC[i][j]+=inputArrayB[i][j];
			}
		}

		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}


	// JOIN THREADS
	for(t=0; t<NUM_THREADS; t++) {
		rc = pthread_join(threads[t], &status);
	}













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
