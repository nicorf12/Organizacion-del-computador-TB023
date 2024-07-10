#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "cache.h"

unsigned penalty = 100;

/* AUX */
unsigned log2_unsigned(unsigned n) {
    if (n == 0) {
        return 0;
    }
    int result = -1;
    while (n > 0) {
        n >>= 1;
        result++;
    }
    return result;
}

bool es_potencia_de_dos(unsigned n) {
    if (n <= 0) {
        return false;
    }
    return (n & (n - 1)) == 0;
}

int bitMask(int highbit, int lowbit) {
  int mask;
  int complement_mask_h;
  int complement_mask_l;
  int equal_low_high;
  int solution;
  mask = ~0x00;
  
  complement_mask_h = mask<<highbit;
  complement_mask_h = complement_mask_h << 1;
  complement_mask_l = ~(mask<<lowbit);
  solution = ~( complement_mask_h | complement_mask_l );
  equal_low_high = !( (highbit) ^ (~(lowbit)) );
  return solution | (equal_low_high<<lowbit);
}

int contar_bytes_hex(unsigned numero) {
    int bytes = 0;

    while (numero > 0) {
        bytes++;
        numero >>= 8;
    }

    return bytes == 0 ? 1 : bytes;
}

/* ESTADISTICAS */
estadisticas_t* estadisticas_new(){
    estadisticas_t* estadisticas = (estadisticas_t*) malloc(sizeof(estadisticas_t));
    estadisticas->lecturas=0;
    estadisticas->escrituras=0;
    estadisticas->rmisses=0;
    estadisticas->wmisses=0;
    estadisticas->dirty_rmiss=0;
    estadisticas->dirty_wmiss=0;
    estadisticas->bytes_read=0;
    estadisticas->bytes_written=0;
    estadisticas->clocks_lectura=0;
    estadisticas->clocks_escritura=0;
    return estadisticas;
}

 /* LINEA */
linea_t* linea_new() {
    linea_t* linea = (linea_t*) malloc(sizeof(linea_t));
    linea->tag = -1;
    linea->valid = false;
    linea->dirty = false;
    linea->last_used = 0;
    return linea;
}

void linea_free(linea_t* linea) {
    free(linea);
}


 /* SET */
set_t* set_new(unsigned vias){
    set_t* set = (set_t*)malloc(sizeof(set_t));
    set -> vias = vias;
    set->lineas = (linea_t**) malloc(vias * sizeof(linea_t*));
    for (unsigned i = 0; i < vias; i++) {
        set->lineas[i] = linea_new();
    }
    return set;
}

void set_free(set_t* set, unsigned vias) {
    for (unsigned i = 0; i < vias; i++) {
        linea_free(set->lineas[i]);
    }
    free(set->lineas);
    free(set);
}

 /* CACHE */
cache_t* cache_new(unsigned vias, unsigned sets, unsigned size){
    cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
    cache->size = size;
    cache->setSize = sets;
    cache->vias = vias;
    cache->bloqueSize = size / (sets * vias); // B =  C / (S * E)
    cache->estadisticas = estadisticas_new();
    cache->sets = (set_t**) malloc(sets * sizeof(set_t*));

    for (unsigned i = 0; i < sets; i ++){
        cache->sets[i] = set_new(vias);
    }
    return cache;
}

void cache_free(cache_t* cache) {
    for (unsigned i = 0; i < cache->setSize; i++) {
        set_free(cache->sets[i], cache->vias);
    }
    free(cache->estadisticas);
    free(cache->sets);
    free(cache);
}

linea_t* cache_hit(cache_t* cache,unsigned set, size_t i, char op_c , unsigned op_num, detalles_t* detalles){
    if (cache->vias > 1) {
        detalles->last_used = cache->sets[set]->lineas[i]->last_used;
    }
    strcpy(detalles->caso, "1");
    detalles->linea=i;
    detalles->last_tag = cache->sets[set]->lineas[i]->tag;
    detalles->valid = cache->sets[set]->lineas[i]->valid;
    detalles->dirty = cache->sets[set]->lineas[i]->dirty;
    
    cache->sets[set]->lineas[i]->last_used = op_num;
    switch (op_c)
        {
        case 'W':
            (cache->estadisticas->clocks_escritura)++;
            break;
        case 'R':
            (cache->estadisticas->clocks_lectura)++;
            break;
        }
    return cache->sets[set]->lineas[i];
}
linea_t* cache_miss(cache_t* cache, set_t* set, unsigned tag, char op_c,unsigned op_num, detalles_t* detalles){
        int min = 0;
        for (size_t i = 0; i < cache->vias; i++)
        {   
            if (!(set->lineas[i]->valid)){
                min = i;
                break;
            }

            if (set->lineas[min]->last_used > set->lineas[i]->last_used){
                min = i;
            }
        }
        linea_t* linea;
        linea = set->lineas[min];

        detalles->linea = min;
        if (linea->dirty) {
            strcpy(detalles->caso, "2b");
        } else {
            strcpy(detalles->caso, "2a");
        }
        detalles->last_tag = -1;
        if (linea->valid) {
            detalles->last_tag=linea->tag;
        }
        detalles->valid = linea->valid;
        detalles->dirty = linea->dirty;
        
        detalles->last_used = linea->last_used;

        (cache->estadisticas->bytes_read)+= cache->bloqueSize;
        switch (op_c)
        {
        case 'W':
            if (linea->dirty) {
                (cache->estadisticas->dirty_wmiss)++;
                cache->estadisticas->clocks_escritura = cache->estadisticas->clocks_escritura + 1 + 2*penalty;
                (cache->estadisticas->bytes_written)+= cache->bloqueSize;
            } else {
                cache->estadisticas->clocks_escritura = cache->estadisticas->clocks_escritura + 1 + penalty;
            }
            (cache->estadisticas->wmisses)++;
            break;
        case 'R':
            if (linea->dirty) {
                (cache->estadisticas->dirty_rmiss)++;
                cache->estadisticas->clocks_lectura = cache->estadisticas->clocks_lectura + 1 + 2*penalty;
                (cache->estadisticas->bytes_written)+= cache->bloqueSize;
            } else {
                cache->estadisticas->clocks_lectura = cache->estadisticas->clocks_lectura + 1 + penalty;
            }
            (cache->estadisticas->rmisses)++;
            break;
        }

        linea->tag = tag;
        linea->last_used = op_num;
        linea->dirty = false;
        linea->valid = true;
        return linea;
}

linea_t* cache_direccion(cache_t* cache, unsigned address, char op_c, unsigned op_num, detalles_t* detalles){
    int b = __builtin_ctz(cache->bloqueSize);
    int s = __builtin_ctz(cache->setSize);
    unsigned set = (address & bitMask(s + b - 1, b)) >> b;
    unsigned tag = (address & bitMask(31, s + b)) >> (s + b);  
    detalles->set = set;
    detalles->tag = tag;

    for (size_t i = 0; i < cache->vias; i++)
    {   
        if (cache->sets[set]->lineas[i]->tag == tag){
            //hit
            return cache_hit(cache,set,i, op_c,op_num,detalles);
        }
    }
    //miss
    return cache_miss(cache, cache->sets[set],tag,op_c,op_num,detalles);
}

void cache_write(cache_t* cache, linea_t* linea){
    (cache->estadisticas->lecturas)++;
    linea->dirty = true;
}
void cache_read(cache_t* cache){
    (cache->estadisticas->escrituras)++;
}

void cache_show(cache_t* cache){
    if (cache->vias > 1) {
        printf("%u-way, ",cache->vias);
    } else {
        printf("direct-mapped, ");
    }
    printf("%u sets, size = %uKB\n", cache->setSize, (cache->size)>>10);
    unsigned accesos = cache->estadisticas->escrituras+cache->estadisticas->lecturas;
    unsigned misses= cache->estadisticas->rmisses+cache->estadisticas->wmisses;
    printf("loads %u stores %u total %u\n",cache->estadisticas->escrituras, cache->estadisticas->lecturas, accesos);
    printf("rmiss %u wmiss %u total %u\n", cache->estadisticas->rmisses,cache->estadisticas->wmisses, cache->estadisticas->rmisses+cache->estadisticas->wmisses);
    printf("dirty rmiss %u dirty wmiss %u\n", cache->estadisticas->dirty_rmiss, cache->estadisticas->dirty_wmiss);
    printf("bytes read %u bytes written %u\n", cache->estadisticas->bytes_read,cache->estadisticas->bytes_written);
    printf("read time %u write time %u\n", cache->estadisticas->clocks_lectura, cache->estadisticas->clocks_escritura);
    printf("miss rate %f\n", ((float)misses)/((float)accesos));
}


/* DETALLES */
detalles_t* detalles_new() {
    detalles_t* detalles = (detalles_t*) malloc(sizeof(detalles_t));
    return detalles;
}

void detalles_reset(detalles_t* detalles, unsigned index_op) {
    detalles->op = index_op;
    strcpy(detalles->caso, "");
    detalles->set=0;
    detalles->tag=0;
    detalles->linea=0;
    detalles->last_tag=-1;
    detalles->valid=true;
    detalles->dirty=false;
    detalles->last_used=0;
}

void detalles_free(detalles_t* detalles) {
    free(detalles);
}

void detalles_show(detalles_t* detalles, cache_t* cache){
    printf("%u %s %x %x %u", detalles->op,detalles->caso, detalles->set, detalles->tag, detalles->linea);
    if (detalles->last_tag == 0xFFFFFFFF) {
        printf(" %i ",-1);
    } else{
        printf(" %x ",detalles->last_tag);
    }
    printf("%u %u",detalles->valid, detalles->dirty);
    if (cache->vias > 1){
        printf(" %u\n",detalles->last_used);
    } else {
        printf("\n");
    }
}