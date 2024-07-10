#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "cache.h"


bool inputs_validos (FILE *file, unsigned sizeCache, unsigned vias, unsigned sets, unsigned n, unsigned m) {
    if (!file) {
        return false;
    }
    if (!es_potencia_de_dos(sizeCache) || !es_potencia_de_dos(vias) || !es_potencia_de_dos(sets)){
        fclose(file);
        return false;
    }

    if (vias*sets > sizeCache ||  sets > sizeCache || vias > sizeCache || !es_potencia_de_dos(sizeCache/(vias*sets))) {
        fclose(file);
        return false;
    }

    
    if ( !(n||m) && m < n){
        fclose(file);
        return false;
    }

    return true;
}

int main (int argc, char* argv[]){
    if (argc != 5 && argc != 8){
        return 1;
    }
    FILE *file;
    file = fopen(argv[1], "r");
    unsigned sizeCache = (unsigned)atoi(argv[2]);
    unsigned vias = (unsigned)atoi(argv[3]);
    unsigned sets = (unsigned)atoi(argv[4]);
    unsigned n,m = 0;
    bool mood_v = false;
    if (argc == 8){
        mood_v = true;
        n = (unsigned)atoi(argv[6]);
        m = (unsigned)atoi(argv[7]);
    }
    if (!inputs_validos(file,sizeCache,vias,sets, n, m)){
        fprintf(stderr, "Error: %s\n", "INPUT INVALIDO");
        return 1;
    }

    cache_t* cache = cache_new(vias, sets, sizeCache);

    unsigned instruccion;
    char operacion;
    unsigned direccionBuscada;
    unsigned tamanioOperacion;
    unsigned date;

    unsigned index_op = 0;
    linea_t* linea;
    detalles_t* detalles = detalles_new();
    int leidos = fscanf(file, "%x: %c %x %u %x\n", &instruccion, &operacion, &direccionBuscada, &tamanioOperacion, &date);
    while (leidos == 5){
        detalles_reset(detalles,index_op);
        linea = cache_direccion(cache,direccionBuscada,operacion,index_op,detalles);
        switch (operacion)
        {
        case 'W':
            cache_write(cache,linea);
            break;
        case 'R':
            cache_read(cache);
            break;
        default:
            break;
        }

        if (index_op >= n && index_op <= m && mood_v) {
            detalles_show(detalles,cache);
        }

        index_op++;
        leidos = fscanf(file, "%x: %c %x %u %x\n", &instruccion, &operacion, &direccionBuscada, &tamanioOperacion, &date);
    }
    fclose(file);
    cache_show(cache);
    detalles_free(detalles);
    cache_free(cache);
    return 0;
}

