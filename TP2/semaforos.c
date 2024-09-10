
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


//Comer ----------------------------------------------------------------

void Comer(char* nombre){
    
    for(int i = 1; i<1933928372918; i++){
        
    }
    printf("%s termino de comer.", nombre);
    
}
//Semaforos ----------------------------------------------------------------

sem_t sem_sentarse;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;


int invitados;
int invitados_sentados = 0;
int invitados_servidos = 0;


// funciones avanzadas ----------------------------------------------------------------

void ManuchoSeSienta(){
printf("manucho se sento\n");
}

void Manucho(){
    sem_wait(&sem_sentarse);
    ManuchoSeSienta();

}

void invitadoSeSienta(){
    printf("invitado se sento\n");
    pthread_mutex_lock(&mutex1);
    invitados_sentados++;
    pthread_mutex_unlock(&mutex1);

    if(invitados_sentados == invitados){
        sem_post(&sem_sentarse);
    }
}



void Invitados(){

    invitadoSeSienta();



}




int main()
{
    printf("Ingresa la cantidad de invitados: ");
    scanf("%d", &invitados);

    int mozos = invitados - 1;

    pthread_t hilos [invitados + mozos + 1];

for(int i = 0; i < invitados; i++){
    pthread_create(&hilos[i], NULL, Invitados, NULL);
}

for(int i = 0; i<mozos; i++){
    pthread_create(&hilos[i+invitados], NULL, Mozos, NULL);
}

pthread_create(&hilos[invitados + mozos + 1], NULL, Manucho, NULL);


    sem_init(&sem_sentarse, 0, 0);

    pthread_create(&hilo1, NULL, Manucho, NULL);
    pthread_create(&hilo2, NULL, invitados, NULL);


    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}
