// #include <mpi.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <iostream>
// using namespace std;

// #define ETIQUETA 288

// #define ORIGEN 0				// Rango origen.
// #define DESTINO_SUP 1			// Rango matriz superior.
// #define DESTINO_INF 2			// Rango matriz inferior.

// #define DEBUG true


// float get_pos(float*, int, int, int);
// float* alocar_matriz(int, int);
// float* crear_matriz_aleatoria(int, int);
// float* crear_matriz_nula(int, int);
// float* multiplicar_matrices(float*, float*, int, int, int);

// void imprimir_matriz(float*, int, int);
// void imprimir_barra(int);


// int main(int argc, char* argv[]) {
// 	int rango,			// ID Proceso.
// 		num_procesos;	// Numero de Procesos.

// 	MPI_Init(&argc, &argv);
// 	MPI_Comm_rank(MPI_COMM_WORLD, &rango);
// 	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

// 	float tiempo_ini,
// 		tiempo_fin;

// 	if (num_procesos < 1) {
// 		printf("Se necesitan al menos dos procesos. Saliendo...\n");
// 		MPI_Abort(MPI_COMM_WORLD, 1);
// 		exit(1);
// 	}

// 	// Semilla para asegurar la aletoriedad.
// 	srand((unsigned)time(NULL));


// 	int tam = 0;
// 	if (rango == ORIGEN) {
// 		// Entrada del usuario del tama�o de la matriz. 
// 		do {
// 			cout << "Introduzca el alto de la matriz cuadrada: ";
// 			cin >> tam;
// 			cout << endl;
// 		} while (tam <= 1);
// 	}

// 	MPI_Bcast(&tam, 1, MPI_INT, ORIGEN, MPI_COMM_WORLD);
// 	// INICIO
// 	tiempo_ini = MPI_Wtime();

// 	// Creacion de matrices.
// 	float* matriz_1 = alocar_matriz(tam, tam);
// 	float* matriz_2 = alocar_matriz(tam, tam);
// 	float* matriz_resultado = alocar_matriz(tam, tam);

// 	if (rango == ORIGEN) {
// 		// Valores aleatorios.
// 		matriz_1 = crear_matriz_aleatoria(tam, tam);
// 		matriz_2 = crear_matriz_aleatoria(tam, tam);
// 		cout << "El proceso 0 ha creado las matrices aleatorias" << endl;
// 	}

// 	// Envio de las matrices
// 	MPI_Bcast(matriz_1, tam * tam, MPI_FLOAT, ORIGEN, MPI_COMM_WORLD);
// 	MPI_Bcast(matriz_2, tam * tam, MPI_FLOAT, ORIGEN, MPI_COMM_WORLD);

// 	float* matriz_res_parcial = alocar_matriz(tam, tam);

// 	// Multiplicacion de matrices.
// 	if (rango != ORIGEN) {

// 		matriz_res_parcial = multiplicar_matrices(matriz_1, matriz_2, tam, num_procesos, rango);

// 		MPI_Send(matriz_res_parcial, tam * tam, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
// 		//free(matriz_res_parcial);
// 	}
// 	else {

// 		MPI_Status estado;

// 		for (int i = 1; i < num_procesos; i++) {
// 			MPI_Recv(matriz_res_parcial, tam * tam, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);

// 			int origen = estado.MPI_SOURCE;

// 			for (int j = 0; j < tam; j++) { // para cada fila
// 				if (j % (num_procesos - 1) + 1 == origen) { // si dicha fila le corresponde al proceso (entre el P01 y P0numprocesos -1)
// 					memcpy(&matriz_resultado[tam * j], &matriz_res_parcial[tam * j], tam * sizeof(float));
// 				}
// 			}


// 		}
// 		//free(matriz_res_parcial);
		
// 	}

// 	// Salida
// 	if (DEBUG && !rango) {
// 		printf("Matriz 1: \n");
// 		imprimir_matriz(matriz_1, tam, tam);
// 		printf("Matriz 2: \n");
// 		imprimir_matriz(matriz_2, tam, tam);
// 		printf("Resultado: \n");
// 		imprimir_matriz(matriz_resultado, tam, tam);
// 	}

// 	// FIN
// 	tiempo_fin = MPI_Wtime();

// 	if (rango == ORIGEN) {
// 		fflush(stdin);
// 		printf("Total time: %f\n", tiempo_fin - tiempo_ini);
// 	}

// 	MPI_Finalize();
// 	return 0;
// }

// float get_pos(float* matriz, int tam, int f, int c) {
// 	if (f > tam || c > tam)
// 		return 0;
// 	return matriz[f * tam + c];
// }

// float* get_columna(float* matriz, int tam, int col) {
// 	float* columna = (float*)malloc(tam * sizeof(float));

// 	for (int i = 0; i < tam; i++) {
// 		columna[i] = get_pos(matriz, tam, i, col);
// 	}

// 	return columna;
// }

// float* get_fila(float* matriz, int tam, int fila) {
// 	return &matriz[fila * tam];
// }

// float fila_por_columna(float* fila, float* columna, int tam) {
// 	float res = 0.0;

// 	for (int i = 0; i < tam; i++) {
// 		res += fila[i] * columna[i];
// 	}

// 	return res;
// }

// float* multiplicar_matrices(float* matriz_1, float* matriz_2, int dimension, int num_procesos, int rango_proceso) {

// 	float* resultado = crear_matriz_nula(dimension, dimension);

// 	for (int i = 0; i < dimension; i++) { // para cada fila
// 		if (i % (num_procesos - 1) + 1 == rango_proceso) { // si dicha fila le corresponde al proceso (entre el P01 y P0numprocesos -1)

// 			float* fila = get_fila(matriz_1, dimension, i);

// 			for (int j = 0; j < dimension; j++) {				// Calcular para cada columna

// 				float* columna = get_columna(matriz_2, dimension, j);

// 				resultado[dimension * i + j] = fila_por_columna(fila, columna, dimension);
// 				free(columna);
// 			}
// 		}
// 	}
// 	return resultado;
// }

// float* alocar_matriz(int filas, int columnas) {
// 	int tam_matriz = filas * columnas;
// 	float* matriz = (float*)malloc(tam_matriz * sizeof(float));
// 	return matriz;
// }

// float* crear_matriz_nula(int filas, int columnas) {
// 	float* matriz = alocar_matriz(filas, columnas);
// 	for (int i = 0; i < filas * columnas; i++)
// 		matriz[i] = 0;

// 	return matriz;
// }

// float* crear_matriz_aleatoria(int filas, int columnas) {

// 	float* matriz = alocar_matriz(filas, columnas);

// 	for (int i = 0; i < filas * columnas; i++) {
// 		float random = ((float)rand() / (float)RAND_MAX) * 10; // float aleatorio entre 1 y 10
// 		matriz[i] = random;
// 	}

// 	return matriz;
// }

// void imprimir_matriz(float* matriz, int filas, int columnas)
// {
// 	imprimir_barra(columnas);
// 	for (int i = 0; i < filas * columnas; i++)
// 	{
// 		printf("|%6.2f", matriz[i]);
// 		// Cada una de las ultimas columnas
// 		if ((i + 1) % columnas == 0 && i != 0)
// 			printf("|\n");
// 	}
// 	imprimir_barra(columnas);
// }

// void imprimir_barra(int columnas) {
// 	for (int i = 0; i < columnas; i++)
// 		printf("-------");
// 	printf("-\n");
// }



