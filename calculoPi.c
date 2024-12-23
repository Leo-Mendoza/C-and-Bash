#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	//siendo size la cantidad de procesos involucrados en la ejecucion
	//size es equivalente a los steps en el calculo, por lo que indican su precision

	int rank, size, root;

	//instanciamos la variable a retornar
	float retorno;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//definimos el hilo raiz o maestro

	root = 0;

	//definimos el intervalo

	float intervalo = 1.0 / size;

	//realizamos la sumatoria

	float sum = 0.0;

	float a = (rank + 0.5) * intervalo;

	sum += 4.0 / ( 1.0 + a * a);

	MPI_Reduce(&sum, &retorno, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

	if(rank == root)
	{
		retorno = retorno * intervalo;
		printf("PI = %f\n", retorno);
	}


	MPI_Finalize();

	return 0;
}
