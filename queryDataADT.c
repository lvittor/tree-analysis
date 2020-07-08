#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queryDataADT.h"

/* Estructura que guarda la información necesaria para Q1 y Q2
*/
typedef struct nbhInfo {  
  char * name;            // Nombre del barrio
  size_t trees;           // La cantidad de arboles del mismo
  size_t population;      // Su cantidad de habitantes
} nbhInfo;

// Arreglo dinámico de nbhInfo
typedef struct nbhInfo * nbhArr;

// Nodo de nbhInfo
typedef struct nbhNode {
  nbhInfo info;
  struct nbhNode * tail;
} nbhNode;

// Lista de nbhInfo
typedef struct nbhNode * nbhList;

enum {UNPREPARED = 0, PREPARED = 1};

/* Estructura que almacena información y estructuras para responder las queries
*/
typedef struct queryDataCDT {
    // Para las queries 1 y 2
    nbhList firstNbh;             // Lista de barrios ordenada según criterio variable
    size_t listDim;
    nbhList current;              // Iterador para recorrer la lista
    nbhInfo * nbhArr;             // Arreglo de barrios ordenados alfabeticamente
    size_t arrDim;                
    char currQuery;               // Flag del tipo de Query
    char arrStatus;               // PREPARED si se actualizó el arreglo despues del último addNbh
} queryDataCDT;

/* --- Queries --- */
// Query 1
// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES}
// Se debe ordenar de manera decreciente por la cantidad de arboles.
// Si son iguales, se prioriza el orden alfabetico de los barrios.
int compareQ1(nbhInfo * info1, nbhInfo * info2){
  int c =  info2->trees - info1->trees;
  if (c != 0)
    return c;
  else
    return strcmp(info1->name, info2->name);
}
