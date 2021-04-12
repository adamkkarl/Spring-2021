#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

#define		NSTEPS	8388600
#define		NITER 	8388600
#define		P_START	0
#define		P_END	10

int main( int argc, char *argv[]){


  int rank, size;

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);



  //
  //I N I T I A L I Z A T I O N S
  //
  area = 0.0;
  struct timeval startTime;
  struct timeval finishTime;


  //Get the start time
  if(rank == 0) {
    gettimeofday(&startTime, NULL);
  }
  //=================================================================

  // DETERMINE REGION THIS PROCESS WILL CALCULATE

  int chunk_size = NSTEPS/size;
  double h = (double)(P_END-P_START)/NSTEPS;

  double start = (double) (P_END-P_START)*rank/size;
  double end = (double) (P_END-P_START)*(rank+1)/size;


  // CALCULATE AREA USING TRAPEZOIDAL METHOD

  //very first height only added once
  if(rank==0) {
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
  if(rank==size-1) {
    f_result = cos(end);
    area -= f_result*h/2;
  }


  // COMBINE AREAS
  if(rank == 0) {

    for (i = 1; i < size; i++) {
      MPI_Recv(&partial_area, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &sts);
      area += partial_area;
    }

  } else {
    MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }

  //=================================================================

  // have main process return solution, calculate time elapsed
  if(rank == 0) {
    gettimeofday(&finishTime, NULL);

    //print solution
    printf("Result : %.2lf \n",area);

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
