#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define printf printf_s
// Cambiar esto por defines en el PC de la UBU.
constexpr int BROADCAST_SOURCE = 0;
constexpr int MIN_RANDOM_VALUE = 1;
constexpr int MAX_RANDOM_VALUE = 100;
constexpr int COMM_DIMENSIONS = 2;
constexpr int REORDER = 1;

int* alocar_matriz(int filas, int columnas) {
	int matrix_linear_space = filas * columnas;
	return (int*)malloc(matrix_linear_space * sizeof(int*));
}

// O(n). Antes O(n^2)
int* create_random_matrix(int filas, int columnas) {
	int* matrix = alocar_matriz(filas, columnas);

	for (int i = 0; i < filas * columnas; i++)
		matrix[i] = rand() % MAX_RANDOM_VALUE + MIN_RANDOM_VALUE;

	return matrix;
}

// O(3n) -> O(n). Antes O(n^2)
void imprimir_barra(int columnas) {
	for (int i = 0; i < columnas; i++)
		printf("-------");

	printf("-\n");
}
void imprimir_matriz(int* matriz, int filas, int columnas)
{
	imprimir_barra(columnas);

	for (int i = 0; i < filas * columnas; i++)
	{
		printf("|%5d ", matriz[i]);

		// Cada una de las ultimas columnas
		if ((i + 1) % columnas == 0 && i != 0) {
			printf("|\n");
		}
	}

	imprimir_barra(columnas);
}

int main(int argc, char* argv[])
{
	double tiempo_ini, tiempo_fin;
	int proceso_rango;	// ID Proceso.
	int num_procesos;	// Numero de Procesos.

	int cpu_nombre_len;
	char cpu_nombre[128];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso_rango);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);
	MPI_Get_processor_name(cpu_nombre, &cpu_nombre_len);

	// Creacion Comm 2: 
	int M = 3;
	int N = 3;

	if (num_procesos != M * N) {
		printf("Ejecutar con %d procesos\n", M * N);
		exit(0);
	}

	MPI_Comm comm_2;
	int dimensiones_tam[2] = { M, N };
	int periods = 1;

	MPI_Cart_create(MPI_COMM_WORLD, COMM_DIMENSIONS, dimensiones_tam, &periods, REORDER, &comm_2);

	// Semilla aleatoria para asegurar la aletoriedad.
	srand((unsigned)time(NULL));

	// INIT
	tiempo_ini = MPI_Wtime();

	// Matriz aleatotia.

	int* matrix_1 = alocar_matriz(M, N);
	int* matrix_2 = alocar_matriz(M, N);
	int* matrix_res = alocar_matriz(M, N);

	// Si el proceso es BROADCAST, llena con valores aleatorios.
	if (proceso_rango == BROADCAST_SOURCE)
	{
		matrix_1 = create_random_matrix(M, N);
		matrix_2 = create_random_matrix(M, N);
	}

	// Envio de Matriz: 
	MPI_Bcast(matrix_1, M * N, MPI_INT, BROADCAST_SOURCE, comm_2);
	MPI_Bcast(matrix_2, M * N, MPI_INT, BROADCAST_SOURCE, comm_2);

	// Cada proceso obtiene sus coordenadas de la COMM MATRIX:
	int* coords = alocar_matriz(1, COMM_DIMENSIONS);
	MPI_Cart_coords(comm_2, proceso_rango, *dimensiones_tam, coords);
	int x = coords[0] * M + coords[1];
	int res_suma = matrix_1[x] + matrix_2[x];

	// Gather rows on result.
	MPI_Gather(&res_suma, 1, MPI_INT, matrix_res, 1, MPI_INT, BROADCAST_SOURCE, comm_2);

	// FIN
	tiempo_fin = MPI_Wtime();

	// Salida.
	if (proceso_rango == BROADCAST_SOURCE) {

		imprimir_matriz(matrix_1, M, N);
		imprimir_matriz(matrix_2, M, N);
		imprimir_matriz(matrix_res, M, N);
		printf("\nTotal time: %f\n", tiempo_fin - tiempo_ini);
	}

	return 0;
}
