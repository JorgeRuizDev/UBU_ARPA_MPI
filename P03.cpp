#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define printf printf_s
constexpr int BROADCAST_SOURCE = 0;
constexpr int MIN_RANDOM_VALUE = 1;
constexpr int MAX_RANDOM_VALUE = 100;

int* allocate_vector(int columns) {
	return (int*)malloc(columns * sizeof(int*));
}

// O(n). Antes O(n^2)
int* create_random_vector(int columns) {
	int* vector = allocate_vector(columns);

	for (int i = 0; i < columns; i++)
		vector[i] = rand() % MAX_RANDOM_VALUE + MIN_RANDOM_VALUE;

	return vector;
}

// O(3n) -> O(n). Antes O(n^2)
void print_bar(int columns) {
	for (int i = 0; i < columns; i++)
		printf("---------");

	printf("-\n");
}
void print_vector(int* vector, int columns)
{
	print_bar(columns);

	for (int i = 0; i < columns; i++)
	{
		printf("|%7d ", vector[i]);
	}
	printf("|\n");

	print_bar(columns);
}

int main(int argc, char* argv[])
{
	double ini_time, end_time;
	int process_rank;	// Process ID.
	int process_count;	// Number of processes.

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

	// Array columns.
	int columns = process_count;

	// Random matrix.
	int* vector_1 = allocate_vector(columns);
	int* vector_2 = allocate_vector(columns);

	// If broadcast process, fill array with random values.
	if (process_rank == BROADCAST_SOURCE)
	{
		vector_1 = create_random_vector(columns);
		vector_2 = create_random_vector(columns);
	}

	// Send vectors.
	int received_1, received_2;
	MPI_Scatter(vector_1, 1, MPI_INT, &received_1, 1, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);
	MPI_Scatter(vector_2, 1, MPI_INT, &received_2, 1, MPI_INT, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// Product of recieved vector part.
	int product_result = received_1 * received_2;

	// Add the product.
	int add_result;
	MPI_Reduce(&product_result, &add_result, 1, MPI_INT, MPI_SUM, BROADCAST_SOURCE, MPI_COMM_WORLD);

	// FINISH
	end_time = MPI_Wtime();

	// Output.
	if (process_rank == BROADCAST_SOURCE) {
		print_vector(vector_1, columns);
		print_vector(vector_2, columns);
		printf("\nEl producto escalar de los dos vectores es: %d\n", add_result);
		printf("\nTotal time: %f\n", end_time - ini_time);
	}

	return 0;
}