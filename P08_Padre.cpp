// ARCHIVO HIJO.

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ETIQUETA 288
#define ORIGEN 0				// Rango origen.
#define HIJO "./Hijo.exe"
#define NUM_HIJOS 5
#define scanf scanf_s


int main(int argc, char* argv[]) {
	int rango;					// ID Proceso.
	int num_procesos;			// Numero de Procesos.

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

	MPI_Comm intercom, intracom;

	// --------------------------

	MPI_Comm_spawn(HIJO, MPI_ARGV_NULL, NUM_HIJOS, MPI_INFO_NULL, ORIGEN, MPI_COMM_WORLD, &intercom, MPI_ERRCODES_IGNORE);
	MPI_Intercomm_merge(intercom, 0, &intracom);

	

	char * mensaje_padre = (char*) malloc(50 * sizeof(char));
	strcpy_s(mensaje_padre, 50, "Hola hijo, que tal estas?");

	int long_msg = (int) strlen(mensaje_padre);
	MPI_Bcast(&long_msg, 1, MPI_INT, ORIGEN, intracom);
	MPI_Bcast(mensaje_padre, long_msg, MPI_CHAR, ORIGEN, intracom);
	if (rango == ORIGEN) {
		printf("Padre envia: %s", mensaje_padre);
		fflush(stdout);
	}

	// --------------------------

	MPI_Finalize();
	return 0;
}
