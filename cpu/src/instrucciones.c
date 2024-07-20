#include <instrucciones.h>

void ejecutar_set(CPU_Registers *registros, const char* registro, uint8_t valor) {
    if (strcmp(registro, "AX") == 0) {
        registros->AX = valor;
    } else if (strcmp(registro, "BX") == 0) {
        registros->BX = valor;
    } else if (strcmp(registro, "CX") == 0) {
        registros->CX = valor;
    } else if (strcmp(registro, "DX") == 0) {
        registros->DX = valor;
    } else if (strcmp(registro, "EAX") == 0) {
        registros->EAX = valor;
    } else if (strcmp(registro, "EBX") == 0) {
        registros->EBX = valor;
    } else if (strcmp(registro, "ECX") == 0) {
        registros->ECX = valor;
    } else if (strcmp(registro, "EDX") == 0) {
        registros->EDX = valor;
    } else {
        printf("Registro no reconocido: %s\n", registro);
    }
}

void ejecutar_sum(CPU_Registers *cpu, const char* destino, const char* origen) {
    uint32_t *reg_destino_32 = NULL;
    uint8_t *reg_destino_8 = NULL;
    uint32_t *reg_origen_32 = NULL;
    uint8_t *reg_origen_8 = NULL;

    // Asignar punteros a los registros correspondientes
    if (strcmp(destino, "AX") == 0) reg_destino_8 = &cpu->AX;
    else if (strcmp(destino, "BX") == 0) reg_destino_8 = &cpu->BX;
    else if (strcmp(destino, "CX") == 0) reg_destino_8 = &cpu->CX;
    else if (strcmp(destino, "DX") == 0) reg_destino_8 = &cpu->DX;
    else if (strcmp(destino, "EAX") == 0) reg_destino_32 = &cpu->EAX;
    else if (strcmp(destino, "EBX") == 0) reg_destino_32 = &cpu->EBX;
    else if (strcmp(destino, "ECX") == 0) reg_destino_32 = &cpu->ECX;
    else if (strcmp(destino, "EDX") == 0) reg_destino_32 = &cpu->EDX;

    if (strcmp(origen, "AX") == 0) reg_origen_8 = &cpu->AX;
    else if (strcmp(origen, "BX") == 0) reg_origen_8 = &cpu->BX;
    else if (strcmp(origen, "CX") == 0) reg_origen_8 = &cpu->CX;
    else if (strcmp(origen, "DX") == 0) reg_origen_8 = &cpu->DX;
    else if (strcmp(origen, "EAX") == 0) reg_origen_32 = &cpu->EAX;
    else if (strcmp(origen, "EBX") == 0) reg_origen_32 = &cpu->EBX;
    else if (strcmp(origen, "ECX") == 0) reg_origen_32 = &cpu->ECX;
    else if (strcmp(origen, "EDX") == 0) reg_origen_32 = &cpu->EDX;

    // Realizar la suma si ambos registros son válidos
    if (reg_destino_8 && reg_origen_8) {
        *reg_destino_8 += *reg_origen_8;
        printf("SUM - %s = %d\n", destino, *reg_destino_8);
    } else if (reg_destino_32 && reg_origen_32) {
        *reg_destino_32 += *reg_origen_32;
        printf("SUM - %s = %d\n", destino, *reg_destino_32);
    } else {
        printf("Registro no reconocido: %s o %s\n", destino, origen);
    }
}

void ejecutar_sub(CPU_Registers *cpu, const char* destino, const char* origen) {
    uint32_t *reg_destino_32 = NULL;
    uint8_t *reg_destino_8 = NULL;
    uint32_t *reg_origen_32 = NULL;
    uint8_t *reg_origen_8 = NULL;

    // Asignar punteros a los registros correspondientes
    if (strcmp(destino, "AX") == 0) reg_destino_8 = &cpu->AX;
    else if (strcmp(destino, "BX") == 0) reg_destino_8 = &cpu->BX;
    else if (strcmp(destino, "CX") == 0) reg_destino_8 = &cpu->CX;
    else if (strcmp(destino, "DX") == 0) reg_destino_8 = &cpu->DX;
    else if (strcmp(destino, "EAX") == 0) reg_destino_32 = &cpu->EAX;
    else if (strcmp(destino, "EBX") == 0) reg_destino_32 = &cpu->EBX;
    else if (strcmp(destino, "ECX") == 0) reg_destino_32 = &cpu->ECX;
    else if (strcmp(destino, "EDX") == 0) reg_destino_32 = &cpu->EDX;

    if (strcmp(origen, "AX") == 0) reg_origen_8 = &cpu->AX;
    else if (strcmp(origen, "BX") == 0) reg_origen_8 = &cpu->BX;
    else if (strcmp(origen, "CX") == 0) reg_origen_8 = &cpu->CX;
    else if (strcmp(origen, "DX") == 0) reg_origen_8 = &cpu->DX;
    else if (strcmp(origen, "EAX") == 0) reg_origen_32 = &cpu->EAX;
    else if (strcmp(origen, "EBX") == 0) reg_origen_32 = &cpu->EBX;
    else if (strcmp(origen, "ECX") == 0) reg_origen_32 = &cpu->ECX;
    else if (strcmp(origen, "EDX") == 0) reg_origen_32 = &cpu->EDX;

    // Realizar la resta si ambos registros son válidos
    if (reg_destino_8 && reg_origen_8) {
        *reg_destino_8 -= *reg_origen_8;
        printf("SUB - %s = %d\n", destino, *reg_destino_8);
    } else if (reg_destino_32 && reg_origen_32) {
        *reg_destino_32 -= *reg_origen_32;
        printf("SUB - %s = %d\n", destino, *reg_destino_32);
    } else {
        printf("Registro no reconocido: %s o %s\n", destino, origen);
    }
}

void ejecutar_jnz(CPU_Registers *cpu, const char* registro, uint32_t nueva_instruccion) {
    uint32_t valor_registro = 0;

    // Asignar valor del registro correspondiente
    if (strcmp(registro, "AX") == 0) valor_registro = cpu->AX;
    else if (strcmp(registro, "BX") == 0) valor_registro = cpu->BX;
    else if (strcmp(registro, "CX") == 0) valor_registro = cpu->CX;
    else if (strcmp(registro, "DX") == 0) valor_registro = cpu->DX;
    else if (strcmp(registro, "EAX") == 0) valor_registro = cpu->EAX;
    else if (strcmp(registro, "EBX") == 0) valor_registro = cpu->EBX;
    else if (strcmp(registro, "ECX") == 0) valor_registro = cpu->ECX;
    else if (strcmp(registro, "EDX") == 0) valor_registro = cpu->EDX;
    else {
        printf("Registro no reconocido: %s\n", registro);
        return;
    }

    // Si el valor del registro es distinto de cero, actualizar el PC
    if (valor_registro != 0) {
        cpu->PC = nueva_instruccion;
        printf("JNZ - Registro %s distinto de cero, saltando a instrucción %d\n", registro, nueva_instruccion);
    } else {
        printf("JNZ - Registro %s es cero, no se realiza salto\n", registro);
    }
}



void paquete_kernel_envio_recurso(const char* recurso){

    t_paquete *paquete_kernel_recurso = crear_paquete(PROCESO_WAIT);
    
    agregar_entero_a_paquete32(paquete_kernel_recurso, (strlen(recurso)+1));
    agregar_string_a_paquete(paquete_kernel_recurso, recurso);
    
    enviar_paquete(paquete_kernel_recurso, socketCliente);
    eliminar_paquete(paquete_kernel_recurso);

}

int ejecutar_wait(Proceso *procesoActual, const char* recurso) {
    
    int bloqueado = 0;
    paquete_kernel_envio_recurso(recurso);
    int respuesta = recibir_resultado_recursos();
    if (respuesta == 0) {
        bloqueado = 1;
        mandarPaqueteaKernel(WAIT_BLOCK);
    }
    return bloqueado;
}

void ejecutar_signal(CPU_Registers *cpu, const char* recurso) {
    mandarPaqueteaKernel(PROCESO_SIGNAL);
    char* RecursoSolicitado = strtok(NULL, " ");
    //enviarMensaje(RecursoSolicitado, socketCliente);
    char* Respuesta = (char*) recibir_paquete(socketCliente);

    if(strcmp(Respuesta, "RECHAZADO")==0)
    {
        mandarPaqueteaKernel(EXIT); //validar
    }

}

int recibir_resultado_recursos(){

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    recv(socketCliente, &(paquete->codigo_operacion), sizeof(op_code), 0);
    
    int respuesta;

    switch(paquete->codigo_operacion){
        case WAIT_SUCCESS:
        {   
            respuesta = 1;
            printf("Se pudo hacer el wait \n");
            break;
        }
        case WAIT_BLOCK:
        {
            respuesta = 0;
            printf("No se puede hacer el wait, devuelvo el proceso \n");
            break;
        }
        default:
        {   
            log_error(loggerCpu, "Error el codigo de wait no es correcto");
            break;
        }
      
    }       
    return respuesta;
}


