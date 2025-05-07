#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (key == NULL)return;

    long pos = hash(key, map->capacity);

    while(map->buckets[pos] != NULL){
        if(is_equal(map->buckets[pos]->key, key)){
            return;
        }
        pos = (pos + 1) % map->capacity;
    }

    Pair *nuevo = createPair(key, value);
    map->buckets[pos] = nuevo ;

    map->current = pos;
    map->size++;

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **antiguo = map->buckets ;
    long old_capacity = map->capacity ;
    
    map->capacity *= 2 ;
    map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*)) ;

    map->size = 0 ;

    for(long k = 0 ; k < old_capacity; k++)
    {
        if (antiguo[k] != NULL && antiguo[k]->key != NULL)
        {
            insertMap(map, antiguo[k]->key, antiguo[k]->value);
        }
    }
    free(antiguo);


}


HashMap * createMap(long capacity) {
    HashMap *mapa = (HashMap*)malloc(sizeof(HashMap)) ;
    mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair)) ;
    mapa->size = 0 ;
    mapa->capacity = capacity ;
    mapa->current = -1 ;
    return mapa;
}

void eraseMap(HashMap * map,  char * key) {   
    if(key == NULL)return;

    long pos = hash(key, map->capacity);
    while(map->buckets[pos] != NULL)
    {
        if(is_equal(map->buckets[pos]->key, key))
        {
            map->buckets[pos]->key = NULL ;
            map->size--;
            return;
        }
        pos = (pos + 1) % map->capacity;
    }

}

Pair * searchMap(HashMap * map,  char * key) {   
    if(key == NULL || map == NULL)return NULL;

    long pos = hash(key, map->capacity);

    while(map->buckets[pos] != NULL)
    {
        if(is_equal(map->buckets[pos]->key, key))
        {
            map->current = pos ;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
    }


    return NULL;
}

Pair * firstMap(HashMap * map) {
    if (map == NULL || map->size == 0)return NULL;

    for(long k = 0 ; k < map->capacity; k++)
    {
        if (map->buckets[k] != NULL && map->buckets[k]->key != NULL)
        {
            map->current = k ;
            return map->buckets[k];
        }
    }

    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL || map->current < 0 || map->current >= map->capacity - 1) return NULL;

    for (long k = map->current + 1; k < map->capacity; k++) {
        if (map->buckets[k] != NULL && map->buckets[k]->key != NULL) {
            map->current = k; 
            return map->buckets[k]; 
        }
    }
    
    return NULL; 
}