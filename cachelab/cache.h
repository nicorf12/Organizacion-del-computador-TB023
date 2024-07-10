#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct{
    bool valid;
    bool dirty;
    char caso[10];
    unsigned op;
    unsigned set;
    unsigned tag;
    unsigned linea;
    unsigned last_tag;
    unsigned last_used;
} detalles_t;

typedef struct{
    unsigned lecturas;
    unsigned escrituras;
    unsigned rmisses;
    unsigned wmisses;
    unsigned dirty_rmiss;
    unsigned dirty_wmiss;
    unsigned bytes_read;
    unsigned bytes_written;
    unsigned clocks_lectura; 
    unsigned clocks_escritura;
} estadisticas_t;

typedef struct linea{
    unsigned tag;
    bool valid;
    bool dirty;
    unsigned last_used;
} linea_t;

typedef struct set{
    unsigned vias;
    linea_t** lineas; 
} set_t;

typedef struct cache{
    unsigned size;
    unsigned setSize;
    unsigned vias;
    unsigned bloqueSize;
    estadisticas_t* estadisticas;
    set_t** sets;
} cache_t;

// Declaraciones de funciones
/* CACHE */
cache_t* cache_new(unsigned vias, unsigned sets, unsigned size);
void cache_free(cache_t* cache);
linea_t* cache_direccion(cache_t* cache, unsigned address,char op_c, unsigned op_num,detalles_t* detalles);
void cache_write(cache_t* cache, linea_t* linea);
void cache_read(cache_t* cache);
void cache_show(cache_t* cache);
/* AUXILIARES */
bool es_potencia_de_dos(unsigned n);
int contar_bytes_hex(unsigned numero);
/* MODO VERBOSO */
detalles_t* detalles_new();
void detalles_reset(detalles_t* detalles, unsigned index_op);
void detalles_free(detalles_t* detalles);
void detalles_show(detalles_t* detalles,cache_t* cache);

#endif 