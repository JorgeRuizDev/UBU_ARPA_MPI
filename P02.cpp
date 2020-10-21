#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define printf printf_s
constexpr int BROADCAST_SOURCE = 0;
constexpr int MIN_RANDOM_VALUE = 1;
constexpr int MAX_RANDOM_VALUE = 100;

int* allocate_matrix(int rows, int columns) {
	int matrix_linear_space = rows * columns;
	return (int*)malloc(matrix_linear_space * sizeof(int*));
}

// O(n). Antes O(n^2)
int* create_random_matrix(int rows, int columns) {
	int* matrix = allocate_matrix(rows, columns);

	for (int i = 0; i < rows * columns; i++)
		matrix[i] = rand() % MAX_RANDOM_VALUE + MIN_RANDOM_VALUE;

	return matrix;
}

// O(3n) -> O(n). Antes O(n^2)
void print_bar(int columns) {
	for (int i = 0; i < columns; i++)
		printf("-------");

	printf("-\n");
}
void print_matrix(int* matrix, int rows, int columns)
{
	print_bar(columns);

	for (int i = 0; i < rows * columns; i++)
	{
		printf("|%5d ", matrix[i]);

		// Every last column.
		if (i % columns == 9 && i != 1) {
			printf("|\n");
		}
	}

	print_bar(columns);
}

int main(int argc, char* argv[])
{
	double ini_time, end_time;
	int process_rank;	 // Process ID.
	int process_count; // Number of processes.

	int cpu_name_length;
	char cpu_name[128];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	MPI_Get_processor_name(cpu_name, &cpu_name_length);

	// Random seed to ensure randomness.
	srand((unsigned)time(NULL));

	// INIT
	ini_time = MPI_Wtime();

	int rows = process_count;
	int columns = 10;

	// Random matrix.
	int* matrix_1 = allocate_matrix(rows, columns);
	int* matrix_2 = allocate_matrix(rows, columns);
	int* matrix_res = allocate_matrix(rows, columns);

	// If broadcast process, fill array with random values.
	if (process_rank == BROADCAST_SOURCE)
	{
		matrix_1 = create_random_matrix(rows, columns);
		matrix_2 = create_random_matrix(rows, columns);
	}

	MPI_Bcast(matrix_1, rows * columns, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);
	MPI_Bcast(matrix_2, rows * columns, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// Every process calculates its own row.
	int* row_addition_res = allocate_matrix(1, columns);
	for (int i = 0; i < columns; i++) {
		int index = process_rank * columns + i;
		row_addition_res[i] = matrix_1[index] + matrix_2[index];
	}

	// Gather rows on result.
	MPI_Gather(row_addition_res, columns, MPI_INT, matrix_res, columns, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// FINISH
	end_time = MPI_Wtime();

	//Output:
	if (process_rank == BROADCAST_SOURCE) {
		print_matrix(matrix_1, rows, columns);
		print_matrix(matrix_2, rows, columns);
		print_matrix(matrix_res, rows, columns);

		printf("Total time: %f\n", end_time - ini_time);
	}
}
