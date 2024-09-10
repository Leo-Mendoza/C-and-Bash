# include  <stdio.h>
# include <pthread.h>
# include <semaphore.h>

int m = 3;              //Cant de mozos
int n = 4;              //Cant de invitados
int platosServidos = 0;
int comenzal_que_responde;


//Semaforos a utilizar
sem_t manuchoSentado;
sem_t manuchoComiendo;
sem_t comenzalesSentados;
sem_t manuchoLanzoPregunta;
sem_t comenzalLanzaRespuesta;

//Funciones secundarias

void trabajo(int multiplicador){
        int i = 0;
        while(i<214748364 * multiplicador){
                i++;
        }
}

void servirComida(int mozo_id){         //Los mozos son menos que los comenzales, los comenzales son los invitados + marucho. A nadie le tengo que servir 2 veces.
        sem_wait(&manuchoSentado);
        trabajo(1);
        platosServidos++;
        printf("Mozo %d sirvio un plato.\n", mozo_id);

        sem_post(&manuchoSentado);
}

void* lanzar_pregunta(){
        //Manucho es el unico que lanza pregunta
        sem_wait(&manuchoComiendo);//Cuando termine de comer manucho
        printf("Manucho: Quien consideran que es el proximo campeon del mundo???");
        sem_post(&manuchoLanzoPregunta);
}

void* lanzar_respuesta(int comenzal_id){
        sem_wait(&manuchoLanzoPregunta);//Si ya se hizo la pregunta
        comenzal_que_responde = *(int*)comenzal_id;     //ATENCION:::::Esto me da violacion de segmento revisar despues de dormir
        printf("Comenzal %d: Francia tiene potencial", comenzal_id);  //Se deberia decir el id del comenzal que responde? quedaria mas bonito? aporta algo?
        sem_post(&comenzalLanzaRespuesta);
}

void* sentarse_manucho(){
        sem_wait(&comenzalesSentados);//Necesito saber si todos los comenzales se sentaron //Quiero ese recurso //Bueno se entiende
        printf("Manucho se sento");
        sem_post(&manuchoSentado);
}

void* enojarse(){
        //El unico que se enoja en nuestro problema es manucho
        //Manucho se enoja cuando se lanza la respuesta
        sem_wait(&comenzalLanzaRespuesta);
        printf("Manucho: -Se enoja por la respuesta!-");
        printf("Manucho: -Se levanta de la mesa!-");
        sem_post(&manuchoSentado);//Manucho se levanto de mesa!
}

void* comer_manucho(){//ESTO esta mal debe comer cuando ya este atendido por un mozo, es solo para probar
        sem_wait(&manuchoSentado);

        printf("Manucho esta comiendo");
        sem_post(&manuchoComiendo);
}

//Esta seria la funcion "principal" de mozos, que deberia hacer llamadas a otras funciones y a semaforos.
void* mozo(void* arg){

        int mozo_id = *(int*)arg;                       //Busco el ID de cada thread
        printf("Mozo %d llego a la fiesta.\n", mozo_id);//Imprimo el ID de cada thread

        servirComida(mozo_id);

        pthread_exit(NULL);                             //Thread termina su laburo
}

//Comportamiento de Manucho.
void* manucho(void* par){
        sentarse_manucho();
        comer_manucho();                //ESte no esta terminado porque aun no esta mozos ni cuando le sirven
        lanzar_pregunta();
        int idfalsa=4;
        lanzar_respuesta(idfalsa);      //SOLO PARA FINGIR UNA RESPUESTA!!!!!!!!! esto no va aca, la respuesta la lanza un comenzal que haya terminado de
        enojarse();

        pthread_exit(NULL);
}

int main (){

        //Defino los threads de los mozos
        pthread_t mozos[m];                             //Array de mozos
        int mozos_id[m];                                //Array que va a contener los ID de los threads
        pthread_t manuchot;

        //Inicializo semaforos y les defino su primer estado
        sem_init(&manuchoSentado, 0, 0);                //Este semaforo se va a usar para que los mozos esperen a que marucho se siente para servir
        sem_init(&comenzalesSentados, 0 ,1);            //debe iniciar en 0 y cuando todos los comenzales esten sentados debe quedar en N
        sem_init(&manuchoComiendo,0,0);                 //No come hasta que no se sienta!
        sem_init(&manuchoLanzoPregunta,0,0);
        sem_init(&comenzalLanzaRespuesta,0,0);


        for(int i = 0; i<m; i++){                       //Instancio los mozos y los mando a hacer su funcion "principal"
                mozos_id[i] = i;                        //Conforme se van asignando threads, voy guardando sus ID
                pthread_create(&mozos[i], NULL, *mozo, &mozos_id[i]); //Inicializo los mozos, los mando a hacer su funcion, y como parametro le paso a cada uno su ID
        }

        for(int i = 0; i<m; i++){                       //Mato a los mozos
                pthread_join(mozos[i], NULL);
        }

        pthread_create(&manuchot, NULL, *manucho, NULL);
        pthread_join(manuchot,NULL);

        printf("Todos los threads deberian haber acabado.\n");

        return 0;
}

/*
void* ejemplo(void* arg){

        //Esta seria la funcion de ejemplo, que deberia hacer llamadas a otras funciones y a semaforos.

        int mozo_id = *(int*)arg;                       //Busco el ID de cada thread
        printf("Thread ID: %d creado! \n", mozo_id);    //Imprimo el ID de cada thread
        pthread_exit(NULL);                             //Thread termina su laburo
}
*/
