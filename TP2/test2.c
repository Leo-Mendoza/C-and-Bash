# include  <stdio.h>
# include <pthread.h>
# include <semaphore.h>
#include <unistd.h>


int N = 3;
int M = 1;

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


void servirComida(int id)
{ //id de invitado
        sem_wait(&mozosDisponibles);    // Espera hasta que un mozo este disponible (disminuye el semaforo)
        printf("\nMozo esta sirviendo al invitado %d.\n", id);
        sleep(1);                       // Simula el tiempo que el mozo tarda en servir al invitado
        printf("Invitado %d esta comiendo.\n", id);
        sleep(3);                       // Simulamos que el invitado esta comiendo
        printf("Invitado %d ha terminado de comer.\n", id);
        sem_post(&unComensalLibre);     // Una vez el invitado termino de comer, lo libero para que haga la pregunta.
        sem_post(&mozosDisponibles);    // Libero al mozo.
}


// ---------------------MANUCHO---------------------------

void* ManuchoSeSienta()
{
    sem_wait(&sem_sentarse);
    printf("\nmanucho se sento\n");
    sem_post(&sem_empezar_servir);
    return NULL;
}


void* ManuchoCome(int id)
{
    servirComida(id);
    return NULL;
}

void* Lanzar_pregunta_mundialista()
{
    printf("\nManucho: Quien consideran que es el proximo campeon del mundo???");
    sem_post(&manuchoLanzoPregunta);
    return NULL;
}

void* enojarse(){
    sem_wait(&comenzalLanzaRespuesta);
    printf("\nManucho: -Se enoja por la respuesta!-");
    printf("\nManucho: -Se levanta de la mesa!-");
    sem_post(&manuchoSeLevanto);//Manucho se levanto de mesa!
    
    return NULL;
}

void* Manucho(void* arg)
{
    int id = *(int*)arg;
    
    ManuchoSeSienta();
    
    ManuchoCome(id);
    
    Lanzar_pregunta_mundialista();
    
    enojarse();
    
    return NULL;

}


// ----------------------INVITADO-------------------------

void* invitadoSeSienta(int id)
{
    printf("\nInvitado %d se sento", id);
    
    pthread_mutex_lock(&mutex1);
    invitados_sentados++;
    pthread_mutex_unlock(&mutex1);

    if(invitados_sentados == N)
    {
    sem_post(&sem_sentarse);
    }
    
    return NULL;
}


void* invitadoCome(int id)
{
    servirComida(id);
    
    return NULL;   
}


void* invitadoResponde(int id){
    
    sem_wait(&manuchoLanzoPregunta);
    
    pthread_mutex_lock(&mutex1);;
    
     if(invitados_comieron == 0)
    {
        printf("\nComenzal %d: Francia tiene potencial", id);
        sem_post(&comenzalLanzaRespuesta);
    }else{
        pthread_mutex_unlock(&mutex1);
        return NULL;
    }
    
}

void* invitadoSeLevanta(int id)
{
    sem_wait(&manuchoSeLevanto);
    printf("Invitado %d se levanta",id);
    return NULL;
}


void* invitado(void* arg){
    
    int id = *(int*)arg;
    
    invitadoSeSienta(id);
    
    invitadoCome(id);
    
    invitadoResponde(id);
    
    invitadoSeLevanta(id);
    
    return NULL;
}


int main()
{
    int ids[N+1]; 
    pthread_t manucho;
    pthread_t invitados [N];
    
    
    //Inicializo semaforos y les defino su primer estado
        sem_init(&sem_empezar_servir, 0, 0);//Este semaforo se va a usar para que los mozos esperen a que marucho se siente para servir
        sem_init(&manuchoLanzoPregunta,0,0);
        sem_init(&comenzalLanzaRespuesta,0,0);
        sem_init(&mozosDisponibles, 0, M);
        sem_init(&unComensalLibre,0,0);
    
    
    pthread_create(&manucho, NULL, (void*)Manucho, &ids[N+1]);
    
    for(int i = 0; i<N; i++)
    {
    ids[i] = i;
    pthread_create(&invitados[i], NULL, (void*)invitado, &ids[i]);
    }
    
    pthread_join(manucho, NULL);
    
    for(int i = 0; i<N; i++)
    {
    pthread_join(invitados[i], NULL);
    }
    
    return 0;
}
