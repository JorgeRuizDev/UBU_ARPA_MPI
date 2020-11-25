// ARCHIVO HIJO.

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ETIQUETA 288
#define ORIGEN 0				// Rango origen.
#define scanf scanf_s

int main1(int argc, char* argv[]) {    
	int rango;					// ID Proceso.
	int num_procesos;			// Numero de Procesos.

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

    MPI_Comm intercom, intracom;

	// --------------------------

    MPI_Comm_get_parent(&intercom);
	MPI_Intercomm_merge(intercom, 1, &intracom);

    char * mensaje_local = (char*)malloc(50 * sizeof(char));
    
	if (rango == ORIGEN) {
		int long_msg = 0;
        MPI_Bcast(&long_msg, 1, MPI_INT, ORIGEN, intracom);

		char* mensaje = (char*)malloc(long_msg * sizeof(char));
        MPI_Bcast(mensaje, long_msg, MPI_CHAR, ORIGEN, intracom);
	    printf("Hijo [%d] Recibe Mensaje de Padre = %s\n", rango, mensaje);

        strcpy_s(mensaje_local,50, "Hola hermano, Me dejah la play?");
        
	}

    MPI_Bcast(mensaje_local, 50, MPI_CHAR, ORIGEN, MPI_COMM_WORLD);
    if (rango != ORIGEN)
        printf("Hijo [%d] Recibe Mensaje de Hermano %d = %s\n", rango, ORIGEN, mensaje_local);
    
    fflush(stdout);

	// --------------------------

	MPI_Finalize();
	return 0;
}

