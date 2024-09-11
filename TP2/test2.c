#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int N = 3;
int M = 2;

sem_t sem_sentarse;
sem_t sem_empezar_servir;
sem_t manuchoLanzoPregunta;
sem_t manuchoSeLevanto;
sem_t comenzalLanzaRespuesta;
sem_t mozosDisponibles;
sem_t unComensalLibre;

pthread_mutex_t mutex1;

int invitados_sentados = 0;
int invitados_comieron = 0;

void servirComida(int id) {
    sem_wait(&mozosDisponibles);    // Espera hasta que un mozo esté disponible
    printf("\nMozo está sirviendo al invitado %d.\n", id);
    sleep(1);                       // Simula el tiempo que el mozo tarda en servir
    printf("\nInvitado %d está comiendo.\n", id);
    sleep(3);                       // Simula el tiempo que el invitado está comiendo
    printf("\nInvitado %d ha terminado de comer.\n", id);
    sem_post(&unComensalLibre);     // Una vez el invitado terminó de comer, lo libero
    sem_post(&mozosDisponibles);    // Libero al mozo
}

// ---------------------MANUCHO---------------------------

void* ManuchoSeSienta(void* arg) {
    sem_wait(&sem_sentarse);
    printf("\nManucho se sentó\n");
    sem_post(&sem_empezar_servir);
    return NULL;
}

void* ManuchoCome(void* arg) {
    int id = *(int*)arg;
    servirComida(id);
    return NULL;
}

void* Lanzar_pregunta_mundialista(void* arg) {
    printf("\nManucho: ¿Quién considera que es el próximo campeón del mundo?\n");
    sem_post(&manuchoLanzoPregunta);
    return NULL;
}

void* enojarse(void* arg) {
    sem_wait(&comenzalLanzaRespuesta);
    printf("\nManucho: -Se enoja por la respuesta!-\n");
    printf("\nManucho: -Se levanta de la mesa!-\n");
    sem_post(&manuchoSeLevanto);  // Manucho libera el semáforo, indicando que se ha levantado
    return NULL;
}

void* Manucho(void* arg) {
    int id = *(int*)arg;
    ManuchoSeSienta(NULL);
    ManuchoCome(&id);
    Lanzar_pregunta_mundialista(NULL);
    enojarse(NULL);
    return NULL;
}

// ----------------------INVITADO-------------------------

void* invitadoSeSienta(void* arg) {
    int id = *(int*)arg;
    printf("\nInvitado %d se sentó\n", id);

    pthread_mutex_lock(&mutex1);
    invitados_sentados++;
    if (invitados_sentados == N) {
        sem_post(&sem_sentarse);
    }
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* invitadoCome(void* arg) {
    int id = *(int*)arg;
    servirComida(id);
    return NULL;
}

void* invitadoResponde(void* arg) {
    int id = *(int*)arg;
    sem_wait(&manuchoLanzoPregunta);

    pthread_mutex_lock(&mutex1);
    if (invitados_comieron < 1) {
        printf("\nComensal %d: ¡Francia tiene potencial!\n", id);
        sem_post(&comenzalLanzaRespuesta);
    }
    invitados_comieron++;
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* invitadoSeLevanta(void* arg) {
    int id = *(int*)arg;
    sem_wait(&manuchoSeLevanto);  // Espera a que Manucho se levante
    printf("\nInvitado %d se levanta\n", id);
    // No es necesario hacer sem_post aquí
    return NULL;
}

void* invitado(void* arg) {
    int id = *(int*)arg;
    invitadoSeSienta(&id);
    invitadoCome(&id);
    invitadoResponde(&id);
    invitadoSeLevanta(&id);
    return NULL;
}

int main() {
    int ids[N+1]; 
    pthread_t manucho;
    pthread_t invitados[N];

    // Inicializo semáforos y mutex
    sem_init(&sem_sentarse, 0, 0);
    sem_init(&sem_empezar_servir, 0, 0);
    sem_init(&manuchoLanzoPregunta, 0, 0);
    sem_init(&manuchoSeLevanto, 0, 0);
    sem_init(&comenzalLanzaRespuesta, 0, 0);
    sem_init(&mozosDisponibles, 0, M);
    sem_init(&unComensalLibre, 0, 0);

    // Creación de hilos
    ids[N] = N; // id para Manucho
    pthread_create(&manucho, NULL, Manucho, &ids[N]);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&invitados[i], NULL, invitado, &ids[i]);
    }

    pthread_join(manucho, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(invitados[i], NULL);
    }

    // Destrucción de semáforos y mutex
    sem_destroy(&sem_sentarse);
    sem_destroy(&sem_empezar_servir);
    sem_destroy(&manuchoLanzoPregunta);
    sem_destroy(&manuchoSeLevanto);
    sem_destroy(&comenzalLanzaRespuesta);
    sem_destroy(&mozosDisponibles);
    sem_destroy(&unComensalLibre);

    pthread_mutex_destroy(&mutex1);

    return 0;
}
