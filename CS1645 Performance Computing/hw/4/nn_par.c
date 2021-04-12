#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

#define		NROW	700
#define		NCOL	NROW

int main(int argc, char* argv[])
{
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

	//Get the start time
	if(rank == 0) {
		gettimeofday(&startTime, NULL);
	}

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

	// DETERMINE CHUNKS FOR THIS PROCESS TO CALCULATE
	int chunk_rows = NROW/size;

	// CALCULATE MATRIX MULTIPLICATION
	//Y = RELU(AVE(XW))
	for (int i=0; i<NROW; i+=size)
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

	// COMBINE SOLUTIONS
	if(rank == 0) {

		for (int i = 1; i < size; i++) {
			double buffer[NROW];

			MPI_Recv(&buffer[0], NROW, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &sts);
			for(int i=0; i<NROW; i++) {
				outputY[i] += buffer[i];
			}
		}
	} else {
		// send all calculated values back to main thread
		MPI_Send(&outputY[0], NROW, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}


	//=====================================================================



	if(rank == 0) {
		// CALCULATE SOFTMAX
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




		//Get the end time
		gettimeofday(&finishTime, NULL);

		//CALCULATE TOTAL SUM
		//[Just for verification]
		val_sum=0;
		for(i=0;i<NROW;i++) {
			val_sum+=(double)Loss[i];
		}

		printf("\nTotal Sum = %g\n",val_sum);


		//Calculate the interval length
		timeIntervalLength = (double)(finishTime.tv_sec-startTime.tv_sec) * 1000000
		+ (double)(finishTime.tv_usec-startTime.tv_usec);
		timeIntervalLength=timeIntervalLength/1000;

		//Print the interval lenght
		printf("Interval length: %g msec.\n", timeIntervalLength);

	}

	MPI_Finalize();
	return 0;
}
