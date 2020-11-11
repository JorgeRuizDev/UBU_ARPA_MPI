#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include <windows.h>

#define ETIQUETA 288
#define ORIGEN 0
#define DESTINO 1

long long unsigned int factorial(int);

/* Method definitions */
int main(int argc, char* argv[]) {
	int rango_proceso;	// ID Proceso.
	int num_procesos;	// Numero de Procesos.

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango_proceso);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

	// Comprobación de procesos:
	if (num_procesos != 2) {
		cout << "Por favor, ejecute únicamente 2 copias del proceso.\n";
		exit(0);
	}

	double tiempo_ini, tiempo_fin;

	tiempo_ini = MPI_Wtime();

	int num;
	long long unsigned fact;
	MPI_Request peticion;
	MPI_Status estado;

	/* The Main program */
	if (rango_proceso == 0) {
		while (true) {
			cout << "Introduce un número: ";
			cin >> num;

			MPI_Send(&num, 1, MPI_INT, DESTINO, ETIQUETA, MPI_COMM_WORLD);

			MPI_Irecv(&fact, 1, MPI_UNSIGNED_LONG_LONG, DESTINO, ETIQUETA, MPI_COMM_WORLD, &peticion);

			int recibido;
			do {
				Sleep(500);
				cout << "Esperando resultado..." << endl;
				MPI_Test(&peticion, &recibido, &estado);
			} while (!recibido);

			cout << "\r Resultado: " << num << "! = " << fact << endl << endl;

			if (num == 0) break;
		}
	}
	else if (rango_proceso == 1) {
		while (true) {
			MPI_Recv(&num, 1, MPI_INT, ORIGEN, ETIQUETA, MPI_COMM_WORLD, &estado);

			fact = factorial(num);
			MPI_Isend(&fact, 1, MPI_UNSIGNED_LONG_LONG, ORIGEN, ETIQUETA, MPI_COMM_WORLD, &peticion);

			if (num == 0) break;
		}
	}

	tiempo_fin = MPI_Wtime();

	if (rango_proceso == ORIGEN)
		printf("Total time: %f\n", tiempo_fin - tiempo_ini);

	MPI_Finalize();
	return 0;
}

long long unsigned int factorial(int number) {
	long long unsigned int result = 1;
	for (int i = 1; i <= number; i++) {
		result *= i;
	}
	return result;
}