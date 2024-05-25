#include <planificadores.h>
#include <pthread.h>
#include <stdio.h>
#include <kernel.h>
#include <semaphore.h>
#include "../src/kernel.c"
#include <queue.h>


// Estructura del proceso

typedef struct proceso {
    PCB pcb_info;
    char *descripcion;
}Proceso;

//Hilos

int main(void) {

    pthread_create(&hilo_largo_plazo, NULL, largo_plazo, NULL);
    pthread_create(&hilo_corto_plazo, NULL, corto_plazo, NULL);

    pthread_join(hilo_largo_plazo, NULL); // detach no te guarda el valor 
    pthread_join(hilo_corto_plazo, NULL);

    return 0;
}

// Semaforos
// Informativo: sem_init(semaforo que inicializo, indica si el sem debe ser compartido entre procesos (0 cuando no), valor inicial del semaforo)

void inicializar_sem_planificadores()
{
	sem_corto_plazo = malloc(sizeof(sem_t));
	sem_init(sem_corto_plazo, 0, 0);

	sem_largo_plazo = malloc(sizeof(sem_t));
	sem_init(sem_largo_plazo, 0, 0);

	sem_grado_multiprogramacion = malloc(sizeof(sem_t));
	sem_init(sem_grado_multiprogramacion, 0, leer_grado_multiprogramación());

	mutex_detener_planificador = malloc(sizeof(sem_t));
	sem_init(mutex_detener_planificador, 0, 1);

    sem_procesos_new = malloc (sizeof(sem_t));
    sem_init(sem_procesos_new, 0, 1);

    sem_procesos_ready = malloc (sizeof(sem_t));
    sem_init(sem_procesos_ready, 0, 1);

    sem_procesos_running = malloc (sizeof(sem_t));
    sem_init(sem_procesos_running, 0, 1);

    sem_proceso_ejecutando = malloc (sizeof(sem_t));
    sem_init(sem_proceso_ejecutando, 0, 0);
}

int leer_grado_multiprogramación() {
    FILE* archivo = fopen ("kernel.config", "r");
    int GRADO_MULTIPROGRAMACION;
}

bool permitePasarAREady() {
    leer_grado_multiprogramacion() > 0;
}


// PLANIFICADOR LARGO PLAZO 
// El proceso pasa de NEW a READY
// Informativo: sem_wait bloquea el hilo si el semaforo se encuentra en 0 o menor a 0. Si el valor del
//semaforo es 1, decrementa el valor (-1) y ejecuta el hilo.

void crearProceso {
    Proceso;
    list_add (lista_NEW, 0); // agregar al final de la lista 
}

void* largo_plazo(void* arg) {
    if (list_size(lista_NEW) != 0)  {

        sem_wait(sem_grado_multiprogramacion); // me fijo que el grado de multiprogramacion este ok
        sem_wait(sem_procesos_new);
        sem_wait(sem_procesos_ready);
        list_remove(lista_NEW, 0); // el 0 indica que se elimina el primer elemento de la lista(como el proceso a analizar es el primero, va estar bien quitarlo de NEW)
        list_add(lista_READY, 0); 
        PCB* Estado = "READY"; // lo agrega al comienzo de la lista, cambiarlo en base a queue
        sem_post(sem_procesos_new);
        sem_post(sem_procesos_ready);
    }
    pthread_exit(NULL);
}

// PLANIFICADOR CORTO PLAZO
// Evaluo que tipo de planificador se debe implementar

void* corto_plazo(void* arg) { // READY - RUNNING - BLOCKED

    if (list_size (lista_READY) != 0) {

    if(strcmp(algoritmo_planificacion, "FIFO") == 0) {
        planificar_fifo();
    } else if (strcmp(algoritmo_planificacion, "RR") == 0) {
        planificar_round_robin();
    } else if (strcmp(algoritmo_planificacion, "VRR") == 0 ) {
        printf ("Algoritmo de planificacion no conocido: %s\n", algoritmo_planificacion);
    }
    }
}

void planificar_fifo() {
    if (list_is_empty(lista_READY)) { // QUEUE *cola_ready
        printf ("No hay procesos en la cola.\n");
        return;
    }
    // variable global con mutex donde definamos que pueda ejecutar wait(mutex)
    // LISTA RUNNING
    sem_wait(sem_proceso_ejecutando);
    sem_wait (sem_procesos_ready); 
    list_remove(lista_READY, 0);
    sem_post (sem_procesos_ready); 
    sem_wait (sem_procesos_running);
    list_add (lista_RUNNING, 0);
    sem_post (sem_procesos_running);
    PCB* Estado = "RUNNING";
    ejecutar_proceso(pcb); //falta implementar esta funcion ( ¿se la pasa a CPU ?)
    // signal (mutex)
    sem_post(sem_proceso_ejecutando);
    free(pcb); //libero memoria

}

/* IDEA PARA LAS INTERRUPCIONES 

interrupt_proceso { 
        if (quantumUsado > quantum) {
        sem_wait(sem_procesos_blocked);
        PCB* Estado = "BLOCKED";
        list_add (lista_BLOCKED, 0);
        sem_wait(sem_procesos_blocked);
        }
    // enviar a la cpu el status del contexto de ejecución
    send (cpu) PCB; // a chequear */

// implementación RR
void planificar_round_robin() {
    if (list_is_empty(cola_de_procesos)) {
        printf ("No hay procesos en la cola.\n");
        return;
    }

PCB* pcb = list_get(cola_de_procesos, 0);

int tiempo_ejecucion = ejecutar_proceso(pcb);

if (tiempo_ejecucion < pcb -> quantum) {
    list_remove(cola_de_procesos, 0); // al haber terminado el proceso dentro del quantum lo elimino de la cola
    free (pcb); // libero memoria del pcb
} else {
    pcb -> pc += tiempo_ejecucion; //calculo el tiempo que ejecuto y cuanto falta, y lo actualizo en el pc
    list_remove(cola_de_procesos, 0);
    list_add(cola_de_procesos, pcb);
}
}