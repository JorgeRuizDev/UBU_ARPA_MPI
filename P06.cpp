#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <iostream>
using namespace std;

#define printf printf_s
#define scanf scanf_s

#define ORIGEN 0
#define DESTINO 1
#define TAG 288

#define scanf scanf_s
int factorial(int n) {
	if (n == 0) {
		return 1;
	}

	int fact = 1;
	for (int i = n; i > 0; i--) {
		fact *= i;
	}
	return fact;
}

int factorialRecursivo(int n) {
	if (n == 0) return 1;
	return n * factorial(n - 1);
}

int lecturaTeclado() {
	int num = 0;
	cout << "Introduzca el valor del factorial a calcular: ";
	cin >> num;
	return num;
}

int operacion(int proceso_rango) {
	int num;
	if (proceso_rango == ORIGEN) {
		num = lecturaTeclado();

		MPI_Send(&num, 1, MPI_INT, DESTINO, TAG, MPI_COMM_WORLD);

		MPI_Request peticion;
		MPI_Status estado;

		int res_factorial;
		MPI_Irecv(&res_factorial, 1, MPI_INT, DESTINO, TAG, MPI_COMM_WORLD, &peticion);

		int recibido;
		do {
			Sleep(500);
			cout << "Estoy esperando" << endl;
			MPI_Test(&peticion, &recibido, &estado);
		} while (!recibido);

		cout << "El factorial de " << num << " es ";
		if (num > 12) cout << "muuuy grande, no puedo calcularlo";
		else cout << res_factorial;
		cout << endl;

		return num;
	}
	else if (proceso_rango == DESTINO) {
		MPI_Status estado;

		int num_rec;
		MPI_Recv(&num_rec, 1, MPI_INT, ORIGEN, TAG, MPI_COMM_WORLD, &estado);

		int res_factorial = factorial(num_rec);
		MPI_Send(&res_factorial, 1, MPI_INT, ORIGEN, TAG, MPI_COMM_WORLD);
	}

	return -1;
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


	// Comprobación de procesos:
	if (num_procesos != 2) {
		cout << "Por favor, ejecute únicamente 2 copias del proceso.\n";
		exit(0);
	}

	// INI
	tiempo_ini = MPI_Wtime();

	int num;
	do {
		num = operacion(proceso_rango);
	} while (num);

	// FIN
	tiempo_fin = MPI_Wtime();

	if (proceso_rango == ORIGEN) {
		printf("Total time: %f\n", tiempo_fin - tiempo_ini);
	}
	MPI_Finalize();
	return 0;
}
