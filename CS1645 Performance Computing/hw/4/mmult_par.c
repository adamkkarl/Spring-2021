#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

#define		NROW   700
#define   NCOL    NROW


int main( int argc, char *argv[]){
  int rank, size;

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);
  MPI_Status sts;


  //
  //I N I T I A L I Z A T I O N S
  //
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


  //=================================================================

  // BUILD MATRICES FOR EVERY PROCESS

  int i,j,k;
  double totalSum;
  double partialSum;

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
  if(rank == 0) {
    gettimeofday(&startTime, NULL);
  }


  // DETERMINE CHUNKS FOR THIS PROCESS TO CALCULATE

  int chunk_rows = NROW/size;

  int start_row = chunk_rows * rank;
  int end_row = start_row + chunk_rows;

  // CALCULATE MATRIX MULTIPLICATION

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


  // COMBINE SOLUTIONS
  if(rank == 0) {

    for (int i = 1; i < size; i++) {

      //determine where received ints go
      start_row = chunk_rows * i;
      end_row = start_row + chunk_rows;

      int num_elements = (end_row-start_row) * NCOL;
      MPI_Recv(&outputArrayC[start_row][0], num_elements, MPI_INT, i, 0, MPI_COMM_WORLD, &sts);

    }
  } else {
    // send all calculated values back to
    int num_elements = (end_row-start_row) * NCOL;
    MPI_Send(&outputArrayC[start_row][0], num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  //=================================================================

  // have main process return solution, calculate time elapsed
  if(rank == 0) {
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


  MPI_Finalize();
  return 0;

}
