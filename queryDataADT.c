#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queryDataADT.h"

static char * strDuplicate(const char * src) {
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    if (dst != NULL)                        // Si hay memoria,
      strcpy(dst, src);                     // hace la copia
    return dst;                             // Retorna el nuevo string
}

static char * allocStringFromSize(size_t n) {
  size_t len = snprintf (NULL, 0, "%lu", n);       // Devuelve la cantidad de caracteres del numero
  char * ans = malloc((len + 1) * sizeof(ans[0])); // Guarda espacio para el nuevo string
  if (ans != NULL)                                 // Si hay memoria,
    snprintf(ans, len + 1, "%lu", n);              // Guarda como string el valor de n
  return ans;                                      // Retorna el nuevo string
}

/* Estructura que guarda la información necesaria para Q1 y Q2
*/
typedef struct nbhInfo {
  char * name;            // Nombre del barrio
  size_t trees;           // La cantidad de arboles del mismo
  size_t population;      // Su cantidad de habitantes
} nbhInfo;

// Arreglo dinámico de nbhInfo
typedef struct nbhInfo * nbhArr;

/* Nodo de nbhInfo
*/
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
    char arrStatus;               // PREPARED si se actualizó el arreglo despues del último addNbh, UNPREPARED sino.
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

char ** infoQ1(nbhInfo nbh) {
  char ** ans = malloc( 2 * sizeof(ans[0]) );
  ans[0] = strDuplicate(nbh.name);
  ans[1] = allocStringFromSize(nbh.trees); /*CANT_DE_ARBOLES a string con sprintf*/
  return ans;
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

// Ordena el vector siguiendo un criterio determinado de acuerdo a la query que se desee (pasandole el mismo como parametro utilizando un puntero a funcion)
static nbhList arrayToList(nbhInfo * arr, size_t size, int (*criteria) (nbhInfo *, nbhInfo *)) {
  int ok = 0; // no se usa en esta funcion
  nbhList first = NULL;
  for (size_t i = 0; i < size; i++) {
    first = addNbhRec(first, arr + i, &ok, criteria);
  }
  return first;
}

//Libera la lista de la estructura
static void freeList(nbhList first){
  nbhList it = first;
  while(it != NULL) {
    nbhList aux = it->tail;
    free(it->info.name);
    free(it);
    it = aux;
  }
}

void toBegin(queryDataADT qd){
  qd->current = qd->firstNbh;
}

void beginQuery1(queryDataADT qd){
  freeList(qd->firstNbh);
  // Crear la lista ordenada de acuerdo al criterio de la query1
  qd->firstNbh = arrayToList(qd->nbhArr, qd->listDim, compareQ1);
  // Setear iterador a head
  toBegin(qd);
  qd->currQuery = 1;
}

void beginQuery2(queryDataADT qd){
  freeList(qd->firstNbh);
  // Crear la lista ordenada de acuerdo al criterio de la query1
  qd->firstNbh = arrayToList(qd->nbhArr, qd->listDim, compareQ2);
  // Setear iterador a head
  toBegin(qd);
  qd->currQuery = 2;
}


int hasNext(queryDataADT qd) {
  return qd->current != NULL;
}


char ** next(queryDataADT qd) {
  char ** ans = malloc( 2 * sizeof(ans[0]) );
  ans[0] = strDuplicate(qd->current->info.name);
  ans[1] = allocStringFromSize(qd->current->info.population);
  qd->current = qd->current->tail;
  return ans;
}


char ** answer(queryDataADT qd, size_t * size) {
  if(! hasNext(qd)){
    fprintf(stderr, "Query data has not next\n");
    exit(1); //no abortar en backend
  }
  char ** ans;
  switch(qd->currQuery) {
    case 1:
      //BARRIO;CANT_ARBOLES
      ans = infoQ1(qd->current->info);
      *size = 2;
      break;
    case 2:
      // BARRIO;CANT_ARBOLES/HABITANTES
      ans = infoQ2(qd->current->info);
      break;
    case 3:
      // NOMBRE_CIENTIFICO;DIAM_PROMEDIO
      //ans = infoQ3(qd->current->info);
      break;
    default:
      return NULL;
  }
  qd->current = qd->current->tail;
  return ans;
}

static void freeArr(nbhArr arr, size_t size){
  for (size_t i = 0; i < size; i++)
    free(arr[i].name);
  free(arr);
}

void freeQueryData(queryDataADT qd) {
  freeList(qd->firstNbh);
  freeArr(qd->nbhArr, qd->arrDim);
  free(qd);
}
