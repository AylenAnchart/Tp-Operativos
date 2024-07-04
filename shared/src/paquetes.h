#ifndef PAQUETES_H
#define PAQUETES_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum
{
    PAQUETE,
    MENSAJE,
    PEDIDO_INSTRUCCION,
    CREAR_PROCESO,
    MOV_IN,
    SET,
    SUM,
    SUB,
    MOV_OUT,
    RESIZE,
    JNZ,
    COPY_STRING,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    EJECUTAR_PROCESO,
    DATOS_DEL_PROCESO,
    DESBLOQUEAR_PROCESO_POR_IO,

    PAUSAR_EJECUCION,
    INTERRUMPIR_PROCESO_ACTUAL

} op_code;

typedef struct
{
    int size;
    void *stream;
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

t_paquete* crear_paquete(op_code codigo_op);
void crear_buffer(t_paquete* paquete);
t_buffer* crear_buffer_aislado(void* data, int data_size);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void agregar_entero_a_paquete32(t_paquete *paquete, uint32_t x);
void agregar_entero_a_paquete8(t_paquete *paquete, uint8_t x);
void agregar_string_a_paquete(t_paquete *paquete, char* palabra);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);
uint32_t leer_entero(char *buffer, int *desplazamiento);
//TODO leer string

#endif // PAQUETES_H