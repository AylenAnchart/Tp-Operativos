#include "utils.h"
#include <cicloInstruccion.h>
#include <cpu.h>

int memoria_fd;
int fd_cpu_dispatch;
int fd_cpu_interrupt;
char* server_name_dispatch = "CPU_DISPATCH";
char* server_name_interrupt = "CPU_INTERRUPT";
t_list* listaPCBS;
t_instruccion instruccionActual;
extern int interrumpir;
op_code codigo_op_hilo_interrupt_cpu;

t_paquete * paqueteProceso(op_code codigoDeOperacion){
    t_paquete *paquete_Kernel = crear_paquete(codigoDeOperacion);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->PID);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.PC);
    agregar_entero_a_paquete8(paquete_Kernel, procesoEjecutando->cpuRegisters.AX);
    agregar_entero_a_paquete8(paquete_Kernel, procesoEjecutando->cpuRegisters.BX);
    agregar_entero_a_paquete8(paquete_Kernel, procesoEjecutando->cpuRegisters.CX);
    agregar_entero_a_paquete8(paquete_Kernel, procesoEjecutando->cpuRegisters.DX);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.EAX);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.EBX);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.ECX);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.EDX);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.SI);
    agregar_entero_a_paquete32(paquete_Kernel, procesoEjecutando->cpuRegisters.DI);

    return paquete_Kernel;

}

void mandarPaqueteaKernel(op_code codigoDeOperacion){
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);
    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
}
void mandarPaqueteaKernelGenerica(op_code codigoDeOperacion, char* nombreInterfaz, int tiempo){
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);

    //ESPECIFICO PARA GENERICA

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(nombreInterfaz)+1));
    agregar_string_a_paquete(paquete_Kernel, nombreInterfaz);
    agregar_entero_a_paquete32(paquete_Kernel, tiempo);

    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
}
void mandarPaqueteaKernelSTD(op_code codigoDeOperacion, char* nombreInterfaz, char *registro1, char *registro2){
    uint32_t dirLog=leerValorDelRegistro(registro1,procesoEjecutando->cpuRegisters);
    uint32_t desplazamientoFisica;
    direccion_fisica* dirFis = traduccion_mmu(desplazamientoFisica,dirLog,procesoEjecutando->PID);

    desplazamientoFisica=(dirFis->numero_frame*tam_pagina)+dirFis->desplazamiento;
    
    uint32_t tamanio=leerValorDelRegistro(registro2,procesoEjecutando->cpuRegisters);
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(nombreInterfaz)+1));
    agregar_string_a_paquete(paquete_Kernel, nombreInterfaz);

    agregar_entero_a_paquete32(paquete_Kernel, desplazamientoFisica);   
    agregar_entero_a_paquete32(paquete_Kernel, tamanio);
    
    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
    free(dirFis);
}
void mandarPaqueteaKernelFScrdel(op_code codigoDeOperacion, char* nombreInterfaz, char *archivo){
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(nombreInterfaz)+1));
    agregar_string_a_paquete(paquete_Kernel, nombreInterfaz);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(archivo)+1));
    agregar_string_a_paquete(paquete_Kernel, archivo);

    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
}
void mandarPaqueteaKernelFStrun(op_code codigoDeOperacion, char* nombreInterfaz, char *archivo, char *registro1){
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(nombreInterfaz)+1));
    agregar_string_a_paquete(paquete_Kernel, nombreInterfaz);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(archivo)+1));
    agregar_string_a_paquete(paquete_Kernel, archivo);

    uint32_t tamanio=leerValorDelRegistro(registro1,procesoEjecutando->cpuRegisters);
    agregar_entero_a_paquete32(paquete_Kernel, tamanio);
    

    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
}
void mandarPaqueteaKernelFSWR(op_code codigoDeOperacion, char* nombreInterfaz, char *archivo, char *registro1, char *registro2, char* registro3){
    uint32_t dirLog=leerValorDelRegistro(registro1,procesoEjecutando->cpuRegisters);
    uint32_t desplazamientoFisica;
    direccion_fisica* dirFis = traduccion_mmu(desplazamientoFisica,dirLog,procesoEjecutando->PID);

    desplazamientoFisica=(dirFis->numero_frame*tam_pagina)+dirFis->desplazamiento;
    
    
    t_paquete *paquete_Kernel = paqueteProceso(codigoDeOperacion);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(nombreInterfaz)+1));
    agregar_string_a_paquete(paquete_Kernel, nombreInterfaz);

    agregar_entero_a_paquete32(paquete_Kernel, (strlen(archivo)+1));
    agregar_string_a_paquete(paquete_Kernel, archivo);

    agregar_entero_a_paquete32(paquete_Kernel, desplazamientoFisica);

    uint32_t tamanio = leerValorDelRegistro(registro2,procesoEjecutando->cpuRegisters);
    agregar_entero_a_paquete32(paquete_Kernel, tamanio);
    
    uint32_t punteroArchivo= leerValorDelRegistro(registro3,procesoEjecutando->cpuRegisters);
    agregar_entero_a_paquete32(paquete_Kernel, punteroArchivo);
    

    enviar_paquete(paquete_Kernel, socketCliente);
    eliminar_paquete(paquete_Kernel);
    free(dirFis);
}