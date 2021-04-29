#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define		ELEMENTS  1024*1024*8
#define   WEIGHT  1024
#define		NUM_THREADS	4


#define TEST_RESULTS


//Input Array A
int inputArrayA  [ELEMENTS];
//Output Array C
int outputArrayC [ELEMENTS];

struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;

void *parallelCalc(void *threadid) {
	long tid;
	tid = (long)threadid;

	int chunk_size = ELEMENTS/NUM_THREADS;

	int start = chunk_size*tid;
	int end = start+chunk_size;

	for(int i=start; i<end; i++)
	{
		outputArrayC[i] = inputArrayA[i]*WEIGHT;
	}
}


int main(int argc, char* argv[])
{
	int i,j,k;
	double totalSum;

	//Get the start time
	gettimeofday(&startTime, NULL); /* START TIME */
	//=========================================================

	//INITIALIZE ARRAYS
	for(i=0;i<ELEMENTS;i++)
	{
		inputArrayA[i]= i*WEIGHT;
		outputArrayC[i]= 0;
	}


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

	//=========================================================
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* END TIME */



	#ifdef TEST_RESULTS
	//CALCULATE TOTAL SUM
	//[Just for verification]
	totalSum=0;
	//
	for(i=0;i<ELEMENTS;i++)
	{
		totalSum+=(double)outputArrayC[i];
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
