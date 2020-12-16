#!/bin/bash
# Script para ejecutar MPI con Linux.

C_FILE=$1
N_CORES=$2
TAM_MATRIX=$3

mpic++ $C_FILE -o executable # Crea ejecutable desde archivo C++ (.cpp).
mpirun -np $N_CORES executable $TAM_MATRIX # Ejecuta con N cores pasados y pasa al archivo el tamaño de la matriz (opcional).
rm executable