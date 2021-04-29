#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <upc_relaxed.h>

#define		ELEMENTS  1024*1024*8
#define   WEIGHT  1024

//Input Array A
shared int inputArrayA [ELEMENTS];
//Output Array C
shared int outputArrayC [ELEMENTS];

int main(int argc, char *argv[]){

  //
  //I N I T I A L I Z A T I O N S
  //


  struct timeval startTime;
  struct timeval finishTime;
  double timeIntervalLength;

  //Get the start time
  if(MYTHREAD == 0) {
    gettimeofday(&startTime, NULL);
  }

  //=================================================================

  // BUILD MATRICES FOR EVERY PROCESS

  int i,j;
  double totalSum;
  double partialSum;

  upc_forall(i=0; i<ELEMENTS; i++; i) {
    inputArrayA[i]= i*WEIGHT;
    outputArrayC[i]= 0;
  }

  // CALCULATE MATRIX MULTIPLICATION

  upc_forall(i=0; i<ELEMENTS; i++; i) {
    outputArrayC[i] = inputArrayA[i] * WEIGHT;
  }

  upc_barrier;

  //=================================================================

  // have main process return solution, calculate time elapsed
  if(MYTHREAD == 0) {
    gettimeofday(&finishTime, NULL);


    //CALCULATE TOTAL SUM
    //[Just for verification]
    totalSum=0;

    for(i=0;i<ELEMENTS;i++)
    {
      totalSum+=(double)outputArrayC[i];
    }
    printf("\nTotal Sum = %g\n",totalSum);

    //Calculate the interval length
    timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000
    + (double)(finishTime.tv_usec-startTime.tv_usec);
    timeIntervalLength=timeIntervalLength/1000;

    //Print the interval length
    printf("Interval length: %g msec.\n", timeIntervalLength);
  }

  return 0;

}
