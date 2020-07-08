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


queryDataADT newQueryData(void){
  return calloc(1, sizeof(queryDataCDT)); //validar NULL
}

static nbhList addNbhRec(nbhList list, nbhInfo * newNbh, int * ok, int (*criteria) (nbhInfo *, nbhInfo *)) {
  int c;
  if (list == NULL || (c = criteria(newNbh, &list->info)) < 0) {
    nbhNode * newNode = malloc(sizeof(nbhNode)); // validar NULL
    newNode->info.trees = newNbh->trees;
    newNode->info.population = newNbh->population;
    newNode->info.name = strDuplicate(newNbh->name);
    newNode->tail = list;
    *ok = 1;
    return newNode;
  }
  if( c > 0 )
    list->tail = addNbhRec(list->tail, newNbh, ok, criteria);
  return list;
}

// Se utiliza en addNbh para dar orden alfabetico a los barrios
static int compareByName(nbhInfo * info1, nbhInfo * info2){
  return strcmp(info1->name, info2->name);
}

void addNbh(queryDataADT qd, char * name, size_t population){
  int ok = 0;
  nbhInfo newNbh = { name, 0, population };
  qd->firstNbh = addNbhRec(qd->firstNbh, &newNbh, &ok, compareByName);
  if(ok){
    qd->arrStatus = UNPREPARED;
    (qd->listDim)++;
  }
}

/* Se pasa la lista a un vector ordenado por names,
*  donde se podrá hacer binSearch para comparar el string e
*  incrementar la cantidad de árboles.
*/
static void listToArray(queryDataADT qd){
  nbhInfo * tmp;
  qd->arrDim = qd->listDim;
  tmp = realloc(qd->nbhArr, qd->arrDim * sizeof(qd->nbhArr[0]));
  if(tmp == NULL){
      fprintf(stderr, "No memory\n");                             // utilizar errno
      exit(1);                                                    // no abortar en backend
  }
  qd->nbhArr = tmp;
  nbhList aux = qd->firstNbh;

  // Se comparte el puntero al nombre del barrio ya que una vez seteado el nombre,
  // no se lo modifica hasta que se libera el TAD
  for(size_t newDim = 0; aux != NULL && newDim < qd->arrDim; newDim++) {
    qd->nbhArr[newDim].population = aux->info.population;
    qd->nbhArr[newDim].trees = aux->info.trees;
    qd->nbhArr[newDim].name = strDuplicate(aux->info.name);

    aux = aux->tail;
  }
}

/* Retorna el indice donde se encontraba nbhName en el arreglo arr.
*  Devuelve -1 si nbhName no está.
*/
static int binSearch(const char * nbhName, nbhInfo * arr, size_t size) {
  int left = 0, right = size - 1;
  while(left <= right) {
    int mid = (left + right) / 2;
    int c = strcmp(nbhName, arr[mid].name);
    if (c == 0)
      return mid;
    if (c > 0)
      left = mid + 1;
    else
      right = mid - 1;
  }
  return -1;
}

// Encuentra el barrio en el vector creado por listToArray e incrementa su campo tree en uno.
void addTree(queryDataADT qd, const char * nbhName){
    if (qd->arrStatus == UNPREPARED){
        qd->arrStatus = PREPARED;
        listToArray(qd);
    }
    int index = binSearch( nbhName, qd->nbhArr, qd->arrDim);

    #if DEBUG
        printf("\nindex: %d\n", index);
    #endif

    if(index >= 0)
        (qd->nbhArr[index].trees)++;
}
