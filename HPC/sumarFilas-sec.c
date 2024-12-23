#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


//funcion para definir el arrar de retorno

int* arrayRetorno(int filasMat)
{

/*Como se estab trabajando en memoria dinamica, se utiliza la funcion calloc, esta crea un array en un bloque de
  memoria de un tamaño determinado por 2 variables, el largo del mismo y el tamaño de cada elemento, e inicializa
  los elementos de este en 0.

  En este caso se define por la cantidad de filas de la matriz y el tamaño en bytes de un int.*/


	int* ret = (int*)calloc(filasMat, sizeof(int));

	//Si hay algun error en la asignacion imprimimos el error y retornamos NULL
	if(ret == NULL)
	{

	printf("Error al asignar memoria\n");
	return NULL;

	}

	return ret;

}

int main(int argc, char* argv[])
{

        //definimos matriz estatica
        int mat[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };

        //variable de suma para todos los hilos en paralelo
        int sum;

        /*
        calculamos la cantidad de filas, sizeof retorna el tamaño en bytes de cualquier dato,
        dividimos el tamaño total por el tamaño de cada
        */

	int filasMat = sizeof(mat) / sizeof(mat[0]);

        //array de retorno

	int* retorno = arrayRetorno(filasMat);

	//variables MPI
        int size,rank, root = 0;

        MPI_Init(&argc, &argv); //Iniciamos paralelismo

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        sum = 0;

        for(int i = 0; i<filasMat; i++)
        {

        sum = sum + mat[rank][i];

        }

        printf("Hilo = %d recorio su fila, Suma = %d", rank, sum);


        if(rank == 0)
        {

                retorno[0] = sum;

                printf("retorno[0] = %d", sum);

                for(int i = 1; i<filasMat; i++)
                {

                sum = 0;
                MPI_Recv(&sum, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
                retorno[i] = sum;

                printf("retorno[%d] = %d", i, sum);

                }
        }

        else{

        MPI_Send(&sum, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

        }

        MPI_Finalize();

        return 0;
}
