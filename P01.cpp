#include <mpi.h>
#include <stdio.h>

#define printf printf_s
#define scanf scanf_s
constexpr int TAG = 288;
constexpr int SOURCE_MACHINE = 0;
constexpr int DEST_MACHINE = 1;

int main(int argc, char* argv[]) {
	int process_rank;	 // Process ID.
	int process_count; // Number of processes.

	int cpu_name_length;
	char cpu_name[128];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
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

	/* Finalize the program */
	MPI_Finalize();
	return 0;
}