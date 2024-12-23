#include <stdio.h>
#include <omp.h>
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

int main()
{

        //definimos matriz estatica
        int mat[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };

	//obtenemos el largo en bytes de cada fila

	int largoFila = sizeof(mat[0]);

        /*
        calculamos la cantidad de filas con sizeof, esta funcion retorna el tamaño en bytes de cualquier dato,
        dividimos el tamaño total de la matriz por el tamaño de cada fila en bytes
        */

        int filasMat = sizeof(mat) / largoFila;

	/*obtenemos la cantidad de ints por fila dividiendo el largo en bytes de la fila por el largo en bytes de
	  una variable int*/

	int elementosEnFila = largoFila / sizeof(int);

        //array de retorno

        int* retorno = arrayRetorno(filasMat);

	//asignamos un proceso paralelo por fila de la matriz
	//omp_set_num_threads(filasMat);

	//iniciamos paralelismo, explicitamos que sum es privada por cada proceso
	#pragma omp parallel
	{
		//inicializamos la variable
       		int sum = 0;

		int rank = omp_get_thread_num();

		//recorremos el largo de una fila, en este caso la 0
		for(int i = 0; i<elementosEnFila; i++)
		{
			//realizamos la suma utilizando el numero de thread como indice de fila
			sum = sum + mat[rank][i];
		}

		retorno[rank] = sum;
	}

	for(int i = 0; i<filasMat; i++)
	{
		printf("Suma fila %d = %d\n", i, retorno[i]);

	}

	return 0;
}
