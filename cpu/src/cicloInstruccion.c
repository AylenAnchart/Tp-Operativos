#include <cicloInstruccion.h>
#include <stdio.h> 
#include <math.h>

extern int memoria_fd; 
char *instruccionRecibida;
extern int program_counter; 
//t_instruccion instruccion;
char memoria[MEM_SIZE][20];
int interrumpir = 0;

void* ciclo_de_instruccion() {
    char *instruccion_a_decodificar;
    int valor= 1;
    t_instruccion instruccion;

    while (valor) {

        instruccion_a_decodificar = fetch(procesoEjecutando);
        printf("Instrucción recibida: %s\n", instruccion_a_decodificar);

        //char **cadena_instruccion = malloc(sizeof(char**));
        char **cadena_instruccion = string_split(instruccion_a_decodificar , " ");
        
        if (strstr(cadena_instruccion[0], "EXIT") != NULL ){

            valor =0;
            mandarPaqueteaKernel(PROCESO_EXIT);
            int tamanio_array = 0;
            while ((cadena_instruccion)[tamanio_array] != NULL) {
                free(cadena_instruccion[tamanio_array]);
                tamanio_array++;
            }
            free(cadena_instruccion);
            free(instruccion_a_decodificar);

            return NULL;
            
        }

        instruccion = decode(instruccion_a_decodificar,procesoEjecutando->PID);

        int bloqueado = execute2(instruccion,procesoEjecutando->PID);

        if(bloqueado == 1){
            bloqueado = 0;
            return NULL;
        }
        if(interrumpir == 2){
            mandarPaqueteaKernel(INTERRUMPIR_PROCESO);

            int tamanio_array = 0;
            while ((cadena_instruccion)[tamanio_array] != NULL) {
                free(cadena_instruccion[tamanio_array]);
                tamanio_array++;
            }
            free(cadena_instruccion);
            free(instruccion_a_decodificar);

            return NULL;
        }
        //mutex interrumpir
        if(interrumpir == 1){
            mandarPaqueteaKernel(PROCESO_INTERRUMPIDO_CLOCK);

            int tamanio_array = 0;
            while ((cadena_instruccion)[tamanio_array] != NULL) {
                free(cadena_instruccion[tamanio_array]);
                tamanio_array++;
            }
            free(cadena_instruccion);
            free(instruccion_a_decodificar);

            return NULL;
        }

        int tamanio_array = 0;
        while ((cadena_instruccion)[tamanio_array] != NULL) {
            free(cadena_instruccion[tamanio_array]);
            tamanio_array++;
        }
        free(cadena_instruccion);
        free(instruccion_a_decodificar);

    }

    return NULL;
}


char* fetch(Proceso *procesoEjecutando) {
    // Obtener la instrucción de la memoria usando el PC
    // Actualizar el PC para la siguiente instrucción

    paquete_memoria_pedido_instruccion(procesoEjecutando->PID,procesoEjecutando->cpuRegisters.PC);
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    recv(memoria_fd, &(paquete->codigo_operacion), sizeof(op_code), 0);
    recv(memoria_fd, &(paquete->buffer->size), sizeof(int), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(memoria_fd, paquete->buffer->stream, paquete->buffer->size, 0);
 

    switch(paquete->codigo_operacion){
            case ENVIO_INSTRUCCION:
            {
                void *stream = paquete->buffer->stream;
                int instruccionLength;

                uint32_t incrementalPC = procesoEjecutando->cpuRegisters.PC +1;
                char *instruccionRecibida;
            
                memcpy(&instruccionLength, stream, sizeof(int));
                stream += sizeof(int);
                instruccionRecibida = malloc(instruccionLength);
                memcpy(instruccionRecibida, stream, instruccionLength);
                procesoEjecutando->cpuRegisters.PC= incrementalPC;
                return instruccionRecibida;
            }
            default:
            {   
                log_error(loggerCpu, "Error");
                break;
            }
    }     

    return NULL;

}

t_instruccion decode(char *instruccionDecodificar, int pid) {

    t_instruccion instruccion;

    char **cadena_instruccion = string_split(instruccionDecodificar , " ");

    int tamanio_array = 0;
    while ((cadena_instruccion)[tamanio_array] != NULL) {
        tamanio_array++;
    }
    if(tamanio_array == 6){
        if (strcmp(cadena_instruccion[0], "IO_FS_WRITE") == 0) {
        
            instruccion.tipo_instruccion = IO_FS_WRITE;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            instruccion.operando3 = cadena_instruccion[3];
            instruccion.operando4 = cadena_instruccion[4];
            instruccion.operando5 = cadena_instruccion[5];

        }
        if (strcmp(cadena_instruccion[0],"IO_FS_READ") == 0) {
        
            instruccion.tipo_instruccion = IO_FS_READ;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            instruccion.operando3 = cadena_instruccion[3];
            instruccion.operando4 = cadena_instruccion[4];
            instruccion.operando5 = cadena_instruccion[5];

        }


    }

    if(tamanio_array == 4){
        if (strcmp(cadena_instruccion[0], "IO_STDIN_READ") == 0) {
        
            instruccion.tipo_instruccion = IO_STDIN_READ;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            instruccion.operando3 = cadena_instruccion[3];

        }
        if (strcmp(cadena_instruccion[0], "IO_STDOUT_WRITE") == 0) {
        
            instruccion.tipo_instruccion = IO_STDOUT_WRITE;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            instruccion.operando3 = cadena_instruccion[3];

        }
        if (strcmp(cadena_instruccion[0], "IO_FS_TRUNCATE") == 0) {
        
            instruccion.tipo_instruccion = IO_FS_TRUNCATE;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            instruccion.operando3 = cadena_instruccion[3];

        }
    }

    if(tamanio_array == 3){

        if (strcmp(cadena_instruccion[0], "MOV_IN") == 0) {

            instruccion.tipo_instruccion = MOV_IN;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
        }

        if (strcmp(cadena_instruccion[0], "MOV_OUT") == 0) {
            
            instruccion.tipo_instruccion = MOV_OUT;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
            
        }

        if (strcmp(cadena_instruccion[0], "SET") == 0) {
            
            instruccion.tipo_instruccion = SET;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];

        }

        if (strcmp(cadena_instruccion[0], "SUM") == 0) {
            
            instruccion.tipo_instruccion = SUM;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];

        }

        if (strcmp(cadena_instruccion[0], "SUB") == 0) {
            
            instruccion.tipo_instruccion = SUB;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];
                        
        }

        if (strcmp(cadena_instruccion[0], "JNZ") == 0) {
            
            instruccion.tipo_instruccion = JNZ;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];

        }
        if (strcmp(cadena_instruccion[0], "IO_GEN_SLEEP") == 0) {
            
            instruccion.tipo_instruccion = IO_GEN_SLEEP;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operandoNumero = atoi(cadena_instruccion[2]);

        }
        if (strcmp(cadena_instruccion[0], "IO_FS_CREATE") == 0) {
        
            instruccion.tipo_instruccion = IO_FS_CREATE;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];

        }
        if (strcmp(cadena_instruccion[0], "IO_FS_DELETE") == 0) {
        
            instruccion.tipo_instruccion = IO_FS_DELETE;
            instruccion.operando1 = cadena_instruccion[1];
            instruccion.operando2 = cadena_instruccion[2];

        }
       
    }

    if(tamanio_array == 2){

        if (strcmp(cadena_instruccion[0], "RESIZE") == 0) {
            
            instruccion.tipo_instruccion = RESIZE;
            instruccion.operando1 = cadena_instruccion[1];
            
        }

        if (strcmp(cadena_instruccion[0], "COPY_STRING") == 0) {
            
            instruccion.tipo_instruccion = COPY_STRING;
            instruccion.operando1 = cadena_instruccion[1];
            
        }

        if (strcmp(cadena_instruccion[0], "WAIT") == 0) {
            
            instruccion.tipo_instruccion = WAIT;
            instruccion.operando1 = cadena_instruccion[1];
            
        }

        if (strcmp(cadena_instruccion[0], "SIGNAL") == 0) {
            
            instruccion.tipo_instruccion = SIGNAL;
            instruccion.operando1 = cadena_instruccion[1];
            
        }
    }

    return instruccion;
    
}
uint32_t leerValorDelRegistro(char *dl,CPU_Registers registros){
	if (strcmp(dl,"AX")==0){
		return registros.AX;
	}else if (strcmp(dl,"BX")==0){
		return registros.BX;
	}else if (strcmp(dl,"CX")==0){
		return registros.CX;
	}else if (strcmp(dl,"DC")==0){
		return registros.DX;
	}else if (strcmp(dl,"EAX")==0){
		return registros.EAX;
	}else if (strcmp(dl,"EBX")==0){
		return registros.EBX;
	}else if (strcmp(dl,"ECX")==0){
		return registros.ECX;
	}else if (strcmp(dl,"EDX")==0){
		return registros.EDX;
	}else if (strcmp(dl,"SI")==0){
		return registros.SI;
	}else if (strcmp(dl,"DI")==0){
		return registros.DI;
	}else {/*TODO ERROR*/}
}

direccion_fisica *traduccion_mmu(uint32_t datos,uint32_t dl, int pid){

    direccion_fisica *direccion = malloc(sizeof(direccion_fisica));

    int nro_pagina;

    nro_pagina = floor(dl / tam_pagina); 
    direccion->PID = pid;

    //logica de paginas divido el tamaño de pagina
    // buscar en memoria el frame y en tlb

    direccion->numero_frame = buscar_frame(nro_pagina,pid);
    direccion->desplazamiento = dl - nro_pagina * tam_pagina;
    
    //char cadena_datos = (char)datos;
    
    return direccion;
}


int obtener_frame_en_tlb(int pid, int pagina){

    int size = list_size(lista_TLB);

    Registro_TLB *reg_TLB = NULL;

    for (int i = 0; i< size; i++) {

        reg_TLB = list_get(lista_TLB,i);

        if (reg_TLB->pid == pid && reg_TLB->pagina == pagina) {

            return reg_TLB->marco;
        }
    }
}

void algoritmoLRU(int pid,int marco_memoria,int pagina){

    int size = list_size(lista_TLB);

    if(size < configuracionCpu.CANTIDAD_ENTRADAS_TLB){
        
        Registro_TLB *reg_TLB = malloc(sizeof(Registro_TLB));
        
        reg_TLB->pid = pid;
        reg_TLB->pagina = pagina;
        reg_TLB->marco = marco_memoria;
        reg_TLB->ultima_modificacion ++;

        list_add(lista_TLB, reg_TLB);
    }

    else if(size == configuracionCpu.CANTIDAD_ENTRADAS_TLB) 

    {   
        int mod=NULL;
        int pid_reemplazar=NULL;

        // obtener el registro menos
        for (int i = 0; i <size; i++){

            Registro_TLB *reg_TLB  = list_get(lista_TLB,i);

            if(i== 0 || reg_TLB->ultima_modificacion < mod)
            {
                mod = reg_TLB->ultima_modificacion;
                pid = reg_TLB->pid;
            }

        }
        
        // reemplazo de registro en tlb si el modificado es el mas alto x el nuevo
        for (int i = 0; i <size; i++){

            Registro_TLB *reg_TLB  = list_get(lista_TLB,i);
            
            if(reg_TLB->pid== pid)
            {
                reg_TLB->pid = pid;
                reg_TLB->pagina = pagina;
                reg_TLB->marco = marco_memoria;
                reg_TLB->ultima_modificacion = 1;
            }

        }

    }

    //ordenerar la lista_TLB segun el ingreso para remover desde la

}

void algoritmoFIFO(int pid,int marco_memoria,int pagina){

    //ordenerar la lista_TLB y eliminar segun ultima modificacion
    
}

void agregar_marco_tlb(int pid,int marco_memoria,int pagina){

    if((strcmp(configuracionCpu.ALGORITMO_TLB,"FIFO")==0))
    {
        algoritmoFIFO(pid,marco_memoria,pagina);

    }

    else if ((strcmp(configuracionCpu.ALGORITMO_TLB,"LRU")==0))
    {

        algoritmoLRU(pid,marco_memoria,pagina);
    }
}

op_code buscar_en_tlb(int pid, int pagina){

    int size = list_size(lista_TLB);

    Registro_TLB *reg_TLB = NULL;

    for (int i = 0; i< size; i++) {

        reg_TLB = list_get(lista_TLB,i);

        if (reg_TLB->pid == pid && reg_TLB->pagina == pagina) {

            return HIT;
        }

    }

    return MISS;
}

int buscar_frame(int pagina, int pid){

    //  PRIMERO: buscar en tlb
    //  SEGUNDO: preguntar a memoria

    switch (buscar_en_tlb(pid,pagina)){
    case HIT:
    {
        int marco_encontrado;
        marco_encontrado = obtener_frame_en_tlb(pid,pagina);
        return marco_encontrado;
    }
    case MISS:
    {
        paquete_memoria_marco(pid,pagina);

        int marco_memoria;
        marco_memoria = recibir_marco_memoria();
        agregar_marco_tlb(pid,marco_memoria,pagina);
        return marco_memoria;
    }
    default:
    
        break;
    }

}

void utilizacion_memoria(t_instruccion instruccion_memoria,int pid){

    //DL
    switch(instruccion_memoria.tipo_instruccion){
        case MOV_IN:
        {   
        
            //MOV_IN (Registro Datos, Registro Dirección): 
            //Lee el valor de memoria correspondiente a la Dirección Lógica que se encuentra en el Registro Dirección y lo almacena en el Registro Datos.
            
            direccion_fisica *direccion_fisica = malloc(sizeof(direccion_fisica));
            
            uint32_t direccion_logica = leerValorDelRegistro(instruccion_memoria.operando2,procesoEjecutando->cpuRegisters);
            uint32_t registro_datos = leerValorDelRegistro(instruccion_memoria.operando1,procesoEjecutando->cpuRegisters);

            direccion_fisica = traduccion_mmu(registro_datos,direccion_logica,pid);
            
            enviar_paquete_mov_in_memoria(direccion_fisica->PID,direccion_fisica->numero_frame,direccion_fisica->desplazamiento);

            uint32_t valor_leido;
            valor_leido = recibir_leer_memoria_mov_in();

            // guardar en registros
            ejecutar_set(&procesoEjecutando->cpuRegisters, instruccion_memoria.operando1, valor_leido);
            break;
        }

        case MOV_OUT:
        {
            //MOV_OUT (Registro Dirección, Registro Datos): 
            //Lee el valor del Registro Datos y 
            //lo escribe en la dirección física de memoria obtenida a partir de la Dirección Lógica almacenada en el Registro Dirección.
    
            direccion_fisica *direccion_fisica = malloc(sizeof(direccion_fisica));

            uint32_t direccion_logica = leerValorDelRegistro(instruccion_memoria.operando1,procesoEjecutando->cpuRegisters);
            uint32_t registro_datos = leerValorDelRegistro(instruccion_memoria.operando2,procesoEjecutando->cpuRegisters);

            op_code operacion;

            direccion_fisica = traduccion_mmu(registro_datos,direccion_logica,pid);

            enviar_paquete_mov_out_memoria(direccion_fisica->PID,direccion_fisica->numero_frame,direccion_fisica->desplazamiento,registro_datos);

            operacion = recibir_confirmacion_memoria_mov_out();

            if(operacion == OK){
                printf("Instrucción resize realizada!! \n");
            }

            break;
        }
        default:
        {   
            log_error(loggerCpu, "Error");
            break;
        }

    }

}

int execute2(t_instruccion instruccion_a_ejecutar,int pid){
    int bloqueado = 0;
    switch(instruccion_a_ejecutar.tipo_instruccion){
        case SET:
        {   
            int valor = atoi(instruccion_a_ejecutar.operando2);
            ejecutar_set(&procesoEjecutando->cpuRegisters, instruccion_a_ejecutar.operando1, valor);
            log_info(loggerCpu, "Se ejecuto la instruccion SET del proceso:%d\n", procesoEjecutando->PID);
            break;
        }
        case SUM:
        {
            ejecutar_sum(&procesoEjecutando->cpuRegisters, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2);
            break;        
        }
        case SUB:
        {
            ejecutar_sub(&procesoEjecutando->cpuRegisters, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2);
            break;
        }
        case JNZ:
        {
            int valor = atoi(instruccion_a_ejecutar.operando2);
            ejecutar_jnz(&procesoEjecutando->cpuRegisters, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2);
            break;
        }
        case WAIT:
        {
            bloqueado = ejecutar_wait(procesoEjecutando, instruccion_a_ejecutar.operando1);

            break;
        }
        case SIGNAL:
            bloqueado = ejecutar_signal(procesoEjecutando, instruccion_a_ejecutar.operando1);
            break;
        case IO_GEN_SLEEP:
        {
            mandarPaqueteaKernelGenerica(IO_GEN_SLEEP, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operandoNumero);
            bloqueado = 1;
            break;
        }
        case IO_STDIN_READ:
        {
            mandarPaqueteaKernelSTD(IO_STDIN_READ, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2,  instruccion_a_ejecutar.operando3);
            bloqueado = 1;
            break;
        }
        case IO_STDOUT_WRITE:
        {
            mandarPaqueteaKernelSTD(IO_STDOUT_WRITE, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2,  instruccion_a_ejecutar.operando3);
            bloqueado = 1;
            break;
        }
        case IO_FS_CREATE:
        {
            mandarPaqueteaKernelFScrdel(IO_FS_CREATE, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2);
            bloqueado = 1;
            break;
        }
        case IO_FS_DELETE:
        {
            mandarPaqueteaKernelFScrdel(IO_FS_DELETE, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2);
            bloqueado = 1;
            break;
        }
        case IO_FS_TRUNCATE:
        {
            mandarPaqueteaKernelFStrun(IO_FS_TRUNCATE, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2, instruccion_a_ejecutar.operando3);
            bloqueado = 1;
            break;
        }
        case IO_FS_WRITE:
        {
            mandarPaqueteaKernelFSWR(IO_FS_WRITE, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2, instruccion_a_ejecutar.operando3,instruccion_a_ejecutar.operando4, instruccion_a_ejecutar.operando5 );
            bloqueado = 1;
            break;
        }
        case IO_FS_READ:
        {
            mandarPaqueteaKernelFSWR(IO_FS_READ, instruccion_a_ejecutar.operando1, instruccion_a_ejecutar.operando2, instruccion_a_ejecutar.operando3,instruccion_a_ejecutar.operando4, instruccion_a_ejecutar.operando5 );
            bloqueado = 1;
            break;
        }
        case MOV_IN:
        {   
            utilizacion_memoria(instruccion_a_ejecutar,pid);
            break;
        }
         case MOV_OUT:
        {
            utilizacion_memoria(instruccion_a_ejecutar,pid);
            break;
        }
        case RESIZE:
        {
            int tam_nuevo = atoi(instruccion_a_ejecutar.operando1);

            paquete_memoria_resize(pid,tam_nuevo);
            op_code operacion;
            operacion=recibir_confirmacion_memoria_resize();
            if(operacion == OUT_OF_MEMORY){
                mandarPaqueteaKernel(EXIT);
                bloqueado = 1;
            }
            printf("Instrucción resize realizada!! \n");
            break;
        }
        default:
        {   
            log_error(loggerCpu, "Error");
            break;
        }

    }
    return bloqueado;     
}

uint32_t recibir_leer_memoria_mov_in(){

    uint32_t valor_leido;

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    
    recv(memoria_fd, &(paquete->codigo_operacion), sizeof(op_code), 0);
    recv(memoria_fd, &(paquete->buffer->size), sizeof(int), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(memoria_fd, paquete->buffer->stream, paquete->buffer->size, 0);

    void *stream = paquete->buffer->stream;

    switch(paquete->codigo_operacion){
        case ENVIO_MOV_IN:
        {
            memcpy(&valor_leido, stream, sizeof(uint32_t));
            return valor_leido;
        }
        default:
        {   
            log_error(loggerCpu, "Error");
            break;
        }
    }       
}

op_code recibir_confirmacion_memoria_mov_out(){

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    recv(memoria_fd, &(paquete->codigo_operacion), sizeof(op_code), 0);
    recv(memoria_fd, &(paquete->buffer->size), sizeof(int), 0);
    switch(paquete->codigo_operacion){
            case OK:
            {
                return OK;
                break;
            }
            default:
            {   
                log_error(loggerCpu, "Error");
                break;
            }

    }
    return PROCESO_EXIT;   

}


op_code recibir_confirmacion_memoria_resize(){

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    recv(memoria_fd, &(paquete->codigo_operacion), sizeof(op_code), 0);
    recv(memoria_fd, &(paquete->buffer->size), sizeof(int), 0);
    switch(paquete->codigo_operacion){
            case OK:
            {
                printf("Instrucción resize realizada!! \n");
                return OK;
                break;
            }
            case OUT_OF_MEMORY:
            {
                //enviar a kernel 
                printf("Instrucción resize: OUT OF MEMORYY ! \n");
                return OUT_OF_MEMORY;
                break;
            }
            default:
            {   
                log_error(loggerCpu, "Error");
                break;
            }

    }
    return PROCESO_EXIT;   

}

int recibir_marco_memoria(){

    int marco_recibido;
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    
    recv(memoria_fd, &(paquete->codigo_operacion), sizeof(op_code), 0);
    recv(memoria_fd, &(paquete->buffer->size), sizeof(int), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(memoria_fd, paquete->buffer->stream, paquete->buffer->size, 0);

    void *stream = paquete->buffer->stream;

    switch(paquete->codigo_operacion){
        case ENVIO_MARCO:
        {
            memcpy(&marco_recibido, stream, sizeof(int));
            return marco_recibido;
        }
        default:
        {   
            log_error(loggerCpu, "Error");
            break;
        }
    }       

}
