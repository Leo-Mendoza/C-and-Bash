# include  <stdio.h>
# include <pthread.h>
# include <semaphore.h>
#include <unistd.h>

sem_t sem_sentarse;
sem_empezar_servir;
sem_t manuchoLanzoPregunta;
sem_t manuchoSeLevanto;
sem_t comenzalLanzaRespuesta;


pthread_mutex_t mutex1;

int invitados_sentados = 0;
int invitados_comieron = 0;

// ---------------------MANUCHO---------------------------

void* ManuchoSeSienta()
{
    sem_wait(&sem_sentarse);
    printf("\nmanucho se sento\n");
    sem_post(&sem_empezar_servir);
    return NULL;
}


void* ManuchoCome()
{
    //Aca espera a que le sirvan
    printf("Manucho esta comiendo");
    Sleep(5);
    return NULL;
}

void* Lanzar_pregunta_mundialista()
{
    printf("\nManucho: Quien consideran que es el proximo campeon del mundo???");
    sem_post(manuchoLanzoPregunta);
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
    //int id = *(int*)arg;
    
    ManuchoSeSienta();
    
    ManuchoCome();
    
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

    if(invitados_sentados == 3)
    {
    sem_post(&sem_sentarse);
    }
    
    return NULL;
}


void* invitadoCome(int id)
{
    // Aca banca a que le sirvan
    
    printf("Invitado %d esta comiendo", id);
    
    Sleep(5);
    
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
    
    //invitadoCome(id);
    
    invitadoResponde(id);
    
    invitadoSeLevanta(id);
    
    return NULL;
}

    
}
int main()
{
    int invitados_id[3]; 
    pthread_t manucho;
    pthread_t invitados [3];
    
    pthread_create(&manucho, NULL, (void*)Manucho, NULL);
    
    for(int i = 0; i<3; i++)
    {
    invitados_id[i] = i;
    pthread_create(&invitados[i], NULL, (void*)invitado, &invitados_id[i]);
    }
    
    pthread_join(manucho, NULL);
    
    for(int i = 0; i<3; i++)
    {
    pthread_join(invitados[i], NULL);
    }
    
    return 0;
}
