#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define printf printf_s
#define FILE_NAME "arpa.txt"
#define COUNT 4

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

	// INIT
	tiempo_ini = MPI_Wtime();

	// Abrimos el fichero: 
	MPI_File fichero;
	MPI_Offset offset = sizeof(char) * COUNT;

	MPI_File_open(MPI_COMM_WORLD, FILE_NAME, MPI_MODE_CREATE | MPI_MODE_RDWR | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &fichero);
	MPI_File_set_view(fichero, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

	char rangos[COUNT];
	for (int i = 0; i < COUNT; i++) {
		rangos[i] = proceso_rango + 48;
	}
	// Escribimos en el fichero
	MPI_Status status;

	//Escribir
	MPI_File_write_at(fichero, offset * proceso_rango, rangos, COUNT, MPI_CHAR, &status);

	char leidos[COUNT];
	//Leer
	MPI_File_read_at(fichero, offset * proceso_rango, leidos, COUNT, MPI_CHAR, &status);

	// Cerrar archivo.
	MPI_File_close(&fichero);

	// FIN
	tiempo_fin = MPI_Wtime();

	printf("Valores escritos por %d: [",proceso_rango);
	for (int i = 0; i < COUNT; i++) {
		printf("%c", leidos[i]);
		if (i != COUNT - 1)
			printf(", ");
	}
	printf("]\n");

	// Salida.
	if(!proceso_rango)
		printf("\nTotal time: %f\n", tiempo_fin - tiempo_ini);

	MPI_Finalize();
	return 0;
}