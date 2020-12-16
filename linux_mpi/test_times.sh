#!/bin/bash

clear
for i in {1..12}
do
    echo "Número de núcleos: $i"
    echo "-----------------------"
    for j in 1000 2000 3000 4000
    do
        echo "> Número de filas/columnas: $j"
        ./run.sh P10.cpp $i $j
    done
done
