#include <omp.h>
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


int main(int argc, char* argv[])
{
	//
	//I N I T I A L I Z A T I O N S
	//
	finalArea = 0.0;


	//Get the start time
	gettimeofday(&startTime, NULL);
	//=================================================================

#pragma omp parallel shared(finalArea)
	{
		/* Obtain thread number */
		int tid = omp_get_thread_num();
		int nthreads = omp_get_num_threads();


		int chunk_size = NSTEPS/nthreads;
		double h = (double)(P_END-P_START)/NSTEPS;

		double start = (double) (P_END-P_START)*tid/nthreads;
		double end = (double) (P_END-P_START)*(tid+1)/nthreads;

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
