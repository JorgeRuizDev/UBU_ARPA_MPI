/*
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define printf printf_s
#define BROADCAST_SOURCE 0

int* allocate_matrix(int x, int y) {
	return (int*) malloc(x * y * sizeof(int*));
}

int* create_random_matrix(int x, int y) {
	int* matrix = allocate_matrix(x, y);

	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			matrix[j + i * y] = rand() % 100 + 1;

	return matrix;
}

void print_matrix(int* matrix, int x, int y) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++)
			printf("%5d ", matrix[i + j * y]);
		printf("\n");
	}
	printf("-----------\n");
}

int main(int argc, char* argv[]) {
	double ini_time, end_time;
	int process_rank;		// Id process
	int process_count;	// number of processes

	int cpu_name_length;
	char cpu_name[128];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	MPI_Get_processor_name(cpu_name, &cpu_name_length);

	// INIT
	ini_time = MPI_Wtime();

	int matrix_size = process_count;

	// Random matrix.
	int * matrix_1 = allocate_matrix(matrix_size, matrix_size);
	int * matrix_2 = allocate_matrix(matrix_size, matrix_size);
	int * matrix_res = allocate_matrix(matrix_size, matrix_size);

	// If broadcast, fill array with random values.
	if (process_rank == BROADCAST_SOURCE) {
		matrix_1 = create_random_matrix(matrix_size, matrix_size);
		matrix_2 = create_random_matrix(matrix_size, matrix_size);

	}



	MPI_Bcast(matrix_1, matrix_size * matrix_size, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);
	MPI_Bcast(matrix_2, matrix_size * matrix_size, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// Every process calculates its own row.
	int* row_addition_res = (int*)malloc(matrix_size * sizeof(int*));
	for (int i = 0; i < matrix_size; i++)
		row_addition_res[i] = matrix_1[process_rank * matrix_size + i] + matrix_2[process_rank * matrix_size + i];

	// Gather rows on result

	MPI_Gather(row_addition_res, matrix_size, MPI_INT, matrix_res, matrix_size, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// FINISH
	end_time = MPI_Wtime();

	//Output:
	if (process_rank == BROADCAST_SOURCE) {
		print_matrix(matrix_1, matrix_size, matrix_size);
		print_matrix(matrix_2, matrix_size, matrix_size);
		print_matrix(matrix_res, matrix_size, matrix_size);

		printf("Total time: %f\n", end_time - ini_time);
	}
}
*/