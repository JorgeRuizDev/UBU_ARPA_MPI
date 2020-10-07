#include <mpi.h>
#include <stdio.h>

#define TAG 288
#define SOURCE_MACHINE 0
#define DEST_MACHINE 1

#define scanf scanf_s

int main(int argc, char ** argv) {
	/* MPI init of vars and funcions */
	int process_rank;		// Id process
	int processes_count;	// number of processes

	int cpu_name_length; 
	char cpu_name[128];
	
	MPI_Init(&argc, &argv);		//Init gloal comm  MPI_COMM_WORLD
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &processes_count);
	MPI_Get_processor_name(cpu_name, &cpu_name_length);

	/* Main program */
	if (process_rank == SOURCE_MACHINE) {
		// Sender:
		int data = 0;
		
		printf("Introduce un entero: ");
		fflush(stdout);
		
		int scanf_count = scanf("%d", &data);
		fflush(stdin);

		MPI_Send(&data, 1, MPI_INT, DEST_MACHINE, TAG, MPI_COMM_WORLD);
		
		printf("[Maquina %s]> El proceso %d ha enviado el mensaje %d.\n", cpu_name, process_rank, data);
		fflush(stdout);
	}
	else if (process_rank == DEST_MACHINE) {
		// Receiver:
		int recieved_data;
		MPI_Status status;

		MPI_Recv(&recieved_data, 1, MPI_INT, SOURCE_MACHINE, TAG, MPI_COMM_WORLD, &status);
		
		printf("[Maquina %s]> El proceso %d ha recibido el mensaje %d\n", cpu_name, process_rank, recieved_data);
		fflush(stdout);
	}

	/* Finalize the  programa */
	MPI_Finalize();
	return 0;
}