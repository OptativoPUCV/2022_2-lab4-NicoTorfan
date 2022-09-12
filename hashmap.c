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

long hash( char * key, long capacity) {
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
  long index = hash(key, map->capacity);
    while (map->buckets[index] != NULL && map->buckets[index]->key != NULL){
        if (is_equal(key, map->buckets[index]->key) == 1){
          return;
        }  
        index= (index+1)% map->capacity;
    }
    if (map->buckets[index] != NULL){
        map->buckets[index]->value= value;
        map->buckets[index]->key= key;
    }
    else map->buckets[index] = createPair(key, value);
    map->size++;
}


void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(capacity*(sizeof(HashMap)));
  map->capacity=capacity;
  map->buckets= (Pair **)malloc(capacity * sizeof(Pair*));
  map->size=0;
  map->current=-1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  long index =hash(key, map->capacity);
  while (map->buckets[index] != NULL && map->buckets[index]->key != NULL){
    if (is_equal(key, map->buckets[index]->key) == 1){
    map->buckets[index]->key=NULL;
    }
    index= (index+1)% map->capacity;
}
  map->size -=1;

}

Pair * searchMap(HashMap * map,  char * key) {   
  long index =hash(key, map->capacity);
  Pair* elemento = NULL;
  while (map->buckets[index] != NULL && map->buckets[index]->key != NULL){
    if (is_equal(key, map->buckets[index]->key) == 1){
      elemento=map->buckets[index];
      map->current=index;
      return elemento;
    }
    index= (index+1)% map->capacity;
}
  return NULL;
}

Pair * firstMap(HashMap * map) {
  map->current=0;
  long index=0;
   while (1){
    if (map->buckets[index] != NULL && map->buckets[index]->key != NULL){
      return map->buckets[index];
    }
    index= (index+1)% map->capacity;
     map->current=index;
  } 
}

Pair * nextMap(HashMap * map) {
  while (map->current <= map->size ){
    map->current=map->current+1;
    if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL){
      return map->buckets[map->current];
    }
}
  return NULL;
}
