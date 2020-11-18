#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_RANDOM_VALUE 10
#define MIN_RANDOM_VALUE 1
#define ETIQUETA 288

#define ORIGEN 0				// Rango origen.
#define DESTINO_SUP 1			// Rango matriz superior.
#define DESTINO_INF 2			// Rango matriz inferior.

#define M 12						// Ancho matriz.
#define N 12						// Alto matriz.

int* alocar_matriz(int, int);
int* crear_matriz_aleatoria(int, int);
int* crear_matriz_nula(int, int);

void imprimir_matriz(int*, int, int);
void imprimir_barra(int);

int main(int argc, char* argv[]) {
	int rango;	// ID Proceso.
	int num_procesos;	// Numero de Procesos.

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

	// Comprobación de procesos:
	if (num_procesos != 3) {
		printf("Por favor, ejecute únicamente 3 copias del proceso.\n");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Semilla para asegurar la aletoriedad.
	srand((unsigned)time(NULL));

	double tiempo_ini, tiempo_fin;

	tiempo_ini = MPI_Wtime();

	// Creación de los 2 nuevos tipos de datos:
	MPI_Datatype MPI_MATRIZ_SUPERIOR;
	MPI_Datatype MPI_MATRIZ_INFERIOR;

	int longitudes_sup[M];
	int desplazamiento_sup[M];

	int longitudes_inf[M];
	int desplazamiento_inf[M];


	for (int i = 0; i < M; i++) {

		longitudes_sup[i] = longitudes_inf[N - 1 - i] = N - i;
		desplazamiento_inf[i] = i * N;
		desplazamiento_sup[i] = i * N + i;

	}

	MPI_Type_indexed(M, longitudes_sup, desplazamiento_sup, MPI_INT, &MPI_MATRIZ_SUPERIOR);
	MPI_Type_commit(&MPI_MATRIZ_SUPERIOR);

	MPI_Type_indexed(M, longitudes_inf, desplazamiento_inf, MPI_INT, &MPI_MATRIZ_INFERIOR);
	MPI_Type_commit(&MPI_MATRIZ_INFERIOR);

	// Creación de matrices y envío: 
	MPI_Status estado;
	MPI_Request peticion;

	switch (rango) {
	case ORIGEN: {
		int* matriz = crear_matriz_aleatoria(M, N);

		MPI_Isend(matriz, 1, MPI_MATRIZ_SUPERIOR, DESTINO_SUP, ETIQUETA, MPI_COMM_WORLD, &peticion);
		MPI_Isend(matriz, 1, MPI_MATRIZ_INFERIOR, DESTINO_INF, ETIQUETA, MPI_COMM_WORLD, &peticion);
		printf("Origen: \n");
		imprimir_matriz(matriz, M, N);
		break;
	}
	case DESTINO_SUP: {
		int* mitad_superior = crear_matriz_nula(M, N);
		MPI_Irecv(mitad_superior, 1, MPI_MATRIZ_SUPERIOR, ORIGEN, ETIQUETA, MPI_COMM_WORLD, &peticion);
		MPI_Wait(&peticion, &estado);
		printf("Superior: \n");
		imprimir_matriz(mitad_superior, M, N);
		break;
	}
	case DESTINO_INF: {
		int* mitad_inferior = crear_matriz_nula(M, N);
		MPI_Irecv(mitad_inferior, 1, MPI_MATRIZ_INFERIOR, ORIGEN, ETIQUETA, MPI_COMM_WORLD, &peticion);
		MPI_Wait(&peticion, &estado);
		printf("Inferior: \n");
		imprimir_matriz(mitad_inferior, M, N);
		break;
	}
	}

	tiempo_fin = MPI_Wtime();

	if (rango == ORIGEN)
		printf("Total time: %f\n", tiempo_fin - tiempo_ini);

	MPI_Finalize();
	return 0;
}

int* alocar_matriz(int filas, int columnas) {
	int matriz_lineal = filas * columnas;
	return (int*)malloc(matriz_lineal * sizeof(int*));
}

int* crear_matriz_nula(int filas, int columnas) {
	int* matriz = alocar_matriz(filas, columnas);
	for (int i = 0; i < filas * columnas; i++)
		matriz[i] = 0;

	return matriz;
}

int* crear_matriz_aleatoria(int filas, int columnas) {
	int* matriz = alocar_matriz(filas, columnas);
	for (int i = 0; i < filas * columnas; i++)
		matriz[i] = rand() % MAX_RANDOM_VALUE + MIN_RANDOM_VALUE;

	return matriz;
}

void imprimir_matriz(int* matriz, int filas, int columnas)
{
	imprimir_barra(columnas);
	for (int i = 0; i < filas * columnas; i++)
	{
		printf("|%5d ", matriz[i]);
		// Cada una de las ultimas columnas
		if ((i + 1) % columnas == 0 && i != 0)
			printf("|\n");
	}
	imprimir_barra(columnas);
}

void imprimir_barra(int columnas) {
	for (int i = 0; i < columnas; i++)
		printf("-------");
	printf("-\n");
}