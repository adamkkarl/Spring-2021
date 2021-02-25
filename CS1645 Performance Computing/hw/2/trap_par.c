#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define		NSTEPS	8388600
#define		NITER 	8388600
#define		P_START	0
#define		P_END	10


struct timeval startTime;
struct timeval finishTime;
double timeIntervalLength;
double finalArea;

int NUM_THREADS = 8;

void *parallelCalc(void *threadid) {
	long tid;
	tid = (long)threadid;

	int chunk_size = NSTEPS/NUM_THREADS;
	double h = (double)(P_END-P_START)/NSTEPS;

	double start = (double) (P_END-P_START)*tid/NUM_THREADS;
	double end = (double) (P_END-P_START)*(tid+1)/NUM_THREADS;

	double area = 0.0;
	double f_result;

	double p_current;

	//very first height only added once
	if(tid==0) {
		f_result = cos(start);
		area -= f_result*h/2;
	}

	//add each height twice
	for(int i = 0; i < chunk_size; i++)
	{
		p_current = start + i*h;
		f_result = cos(p_current);
		area += f_result*h; //2*f_result*h/2 == f_result*h
		p_current += h;
	}

	//very last height only added once
	if(tid==NUM_THREADS-1) {
		f_result = cos(end);
		area -= f_result*h/2;
	}

	finalArea += area;
}


int main(int argc, char* argv[])
{
	//
	//I N I T I A L I Z A T I O N S
	//
	finalArea = 0.0;


	//Get the start time
	gettimeofday(&startTime, NULL);
	//=================================================================

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

	//=================================================================
	//Get the end time
	gettimeofday(&finishTime, NULL);  /* after time */


	printf("Result : %.2lf \n",finalArea);

	//Calculate the interval length
	timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000
		     + (double)(finishTime.tv_usec-startTime.tv_usec);
	timeIntervalLength=timeIntervalLength/1000;

	//Print the interval lenght
	printf("Interval length: %g msec.\n", timeIntervalLength);


	return 0;
}
