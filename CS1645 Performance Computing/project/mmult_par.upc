#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <upc_relaxed.h>

#define		NROW   1024
#define   NCOL   NROW

//Input Array A
shared [NCOL] int inputArrayA [NROW][NCOL];
//Input Array B
shared [NCOL] int inputArrayB [NROW][NCOL];
//Weights
shared int Weight [NROW][NCOL];
//Output Array C
shared  int outputArrayC [NROW][NCOL];

int main(int argc, char *argv[]){

  //
  //I N I T I A L I Z A T I O N S
  //
  struct timeval startTime;
  struct timeval finishTime;
  double timeIntervalLength;


  //=================================================================
  // BUILD MATRICES

  int i,j,k;
  double totalSum;
  double partialSum;

  //INITIALIZE ARRAYS
  for(i=0;i<NROW;i++,i)
  {
    for(j=0;j<NCOL;j++)
    {
      inputArrayA[i][j]= i*NCOL+j;
      inputArrayB[i][j]= j*NCOL+j;
      Weight[i][j]= i*NCOL+j;
      outputArrayC[i][j]= 0;
    }
  }
  upc_barrier;

  //Get the start time
  if(MYTHREAD == 0) {
    gettimeofday(&startTime, NULL);
  }

  // CALCULATE MATRIX MULTIPLICATION

  upc_forall(i=0; i<NROW; i++; & inputArrayA[i][0]) {
    for(j=0; j<NROW; j++) {
      int mysum = inputArrayB[i][j];
      for(k=0; k<NCOL; k++) {
        mysum += inputArrayA[i][k]*Weight[k][j];
      }
      outputArrayC[i][j] = mysum;
    }
  }

  //=================================================================

  upc_barrier;

  // have main process return solution, calculate time elapsed
  if(MYTHREAD == 0) {
    gettimeofday(&finishTime, NULL);


    //CALCULATE TOTAL SUM
    //[Just for verification]
    totalSum=0;

    for(i=0;i<NROW;i++)
    {
      for(j=0;j<NCOL;j++)
      {
        totalSum+=(double)outputArrayC[i][j];
      }
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
