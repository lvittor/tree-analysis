#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "queryDataADT.h"

#define MAX_QUERY 3

#define fabs(n) ((n) < 0 ? -(n) : (n))


static float truncate(float value){
    return (int)(value * 100)/100.0;
}

static char * strDuplicate(const char * src) {
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    if(dst == NULL)                         // Si no hay memoria
        perror("Error en malloc()");        // error.
    else                                    // Si no,
        strcpy(dst, src);                   // hace la copia
    return dst;                             // Retorna el nuevo string
}

static char * allocStringFromSize(size_t n) {
    size_t len = snprintf (NULL, 0, "%lu", n);       // Devuelve la cantidad de caracteres del numero
    char * ans = malloc((len + 1) * sizeof(ans[0])); // Guarda espacio para el nuevo string
    if(ans == NULL)                                  // Si no hay memoria,
        perror("Error en malloc()");                 // error.
    else                                             // Si no,
        snprintf(ans, len + 1, "%lu", n);            // Guarda como string el valor de n
    return ans;                                      // Retorna el nuevo string
}

static char * allocStringFromFloat(float f) {
    f = (int)(f * 100) / 100.0;                       // Trunca a dos decimales
    size_t len = snprintf (NULL, 0, "%.2f", f);       // Devuelve la cantidad de caracteres del numero ya truncado
    char * ans = malloc((len + 1) * sizeof(ans[0]));  // Guarda espacio para el nuevo string
    if(ans == NULL)                                   // Si no hay memoria,
        perror("Error en malloc()");                  // error.
    else                                              // Si no,
        snprintf(ans, len + 1, "%.2f", f);            // Guarda como string el valor de f
    return ans;                                       // Retorna el nuevo string
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

typedef struct treeSpecie{
    char * name;
    float sumDiam;
    size_t count;
}treeSpecie;

typedef struct treeSpecie * treeArr;

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
    // Para la querie 3
    treeArr speciesArr;
    size_t speciesCount;
    size_t itSpecies;
} queryDataCDT;

/* --- Queries --- */
typedef int (*pCompFunc)(const void *, const void *);
// Query 1
// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES}
// Se debe ordenar de manera decreciente por la cantidad de arboles.
// Si son iguales, se prioriza el orden alfabetico de los barrios.
int compareQ1(const void * i1, const void * i2){
    struct nbhInfo * info1 = (struct nbhInfo *)i1;
    struct nbhInfo * info2 = (struct nbhInfo *)i2;
    int c =  info2->trees - info1->trees;
    if (c != 0)
        return c;
    else
        return strcmp( info1->name, info2->name);
}

char ** infoQ1(nbhInfo nbh) {
  char ** ans = malloc( 2 * sizeof(ans[0]) );
  ans[0] = strDuplicate(nbh.name);
  ans[1] = allocStringFromSize(nbh.trees); /*CANT_DE_ARBOLES a string con sprintf*/
  return ans;
}

// Query 2
// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES/CANT_DE_HABITANTES}
// Se debe ordenar de manera decreciente por la cantidad de arboles.
// Si son iguales, se prioriza el orden alfabetico de los barrios.
int compareQ2(const void * i1, const void * i2){
    struct nbhInfo * info1 = (struct nbhInfo *)i1;
    struct nbhInfo * info2 = (struct nbhInfo *)i2;
    float ratio1 = (float)info1->trees/info1->population;
    float ratio2 = (float)info2->trees/info2->population;
    float c = truncate(ratio2) - truncate(ratio1);
    if(fabs(c) < EPSILON)
        return strcmp(info1->name, info2->name);
    else
        return (c > 0) ? 1 : -1;
}

// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES/CANT_DE_HABITANTES}
char ** infoQ2(nbhInfo nbh) {
  char ** ans = malloc( 2 * sizeof(ans[0]) );
  ans[0] = strDuplicate(nbh.name);
  #if DEBUG
    printf("Valor original: %f\n", (float)nbh.trees/nbh.population);
  #endif
  ans[1] = allocStringFromFloat((float)nbh.trees/nbh.population);
  return ans;
}

int compareQ3(const void * s1, const void * s2) {
    struct treeSpecie * specie1 = (struct treeSpecie *)s1;
    struct treeSpecie * specie2 = (struct treeSpecie *)s2;
    float avDiam1 = specie1->sumDiam/specie1->count;
    float avDiam2 = specie2->sumDiam/specie2->count;
    float c = truncate(avDiam2) - truncate(avDiam1);
    if (fabs(c) < EPSILON)
        return strcmp(specie1->name, specie2->name);
    else
        return (c < 0) ? -1 : 1;
}

// ->{NOMBRE_CIENTIFICO_ARBOL, DIAM_PROMEDIO}
char ** infoQ3(treeSpecie tree) {
  char ** ans = malloc( 2 * sizeof(ans[0]) );
  ans[0] = strDuplicate(tree.name);
  ans[1] = allocStringFromFloat(tree.sumDiam/tree.count);
  return ans;
}

queryDataADT newQueryData(void){
  queryDataADT qd;
  if( (qd = calloc(1, sizeof(queryDataCDT))) == NULL )
    perror("Error en calloc()");
  return qd;
}

static nbhList addNbhRec(nbhList list, nbhInfo * newNbh, int * ok, pCompFunc criteria) {
  int c;
  if (list == NULL || (c = criteria(newNbh, &list->info)) < 0) {
    nbhNode * newNode;
    if((newNode = malloc(sizeof(nbhNode))) == NULL ){
      perror("Error en malloc()");
      *ok = ERROR;
      return list;
    }
    newNode->info.trees = newNbh->trees;
    newNode->info.population = newNbh->population;
    newNode->info.name = strDuplicate(newNbh->name);
    newNode->tail = list;
    *ok = SUCCESS;
    return newNode;
  }
  if( c > 0 )
    list->tail = addNbhRec(list->tail, newNbh, ok, criteria);
  return list;
}

// Se utiliza en addNbh para dar orden alfabetico a los barrios
static int compareByName(const void * i1, const void * i2){
  struct nbhInfo * info1 = (struct nbhInfo *)i1;
  struct nbhInfo * info2 = (struct nbhInfo *)i2;
  return strcmp(info1->name, info2->name);
}

void addNbh(queryDataADT qd, char * name, size_t population){
  int ok = !SUCCESS;
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
static int listToArray(queryDataADT qd){
  nbhInfo * tmp;
  qd->arrDim = qd->listDim;
  if((tmp = realloc(qd->nbhArr, qd->arrDim * sizeof(qd->nbhArr[0]))) == NULL){
      perror("Error en realloc()");
      return ERROR;
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
  return SUCCESS;
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

static int addScientificTree(queryDataADT qd, const char * speciesName, float diam) {
    for (size_t i = 0; i < qd->speciesCount; i++) {
        // Si la especia ya estaba guardada,
        if (strcmp(speciesName, qd->speciesArr[i].name) == 0) {
            // Aumenta la cantidad
            qd->speciesArr[i].count++;
            // Actualiza sumatoria de diametros
            qd->speciesArr[i].sumDiam += diam;
            return SUCCESS;
        }
    }
    // Si no habia un ejemplar anterior
    (qd->speciesCount)++;
    treeArr tmp = realloc(qd->speciesArr, (qd->speciesCount) * sizeof(tmp[0]));
    if (tmp == NULL)
        return ERROR;
    qd->speciesArr = tmp;
    qd->speciesArr[qd->speciesCount-1].name = strDuplicate(speciesName);
    qd->speciesArr[qd->speciesCount-1].count = 1;
    qd->speciesArr[qd->speciesCount-1].sumDiam = diam;
    return SUCCESS;
}

// Encuentra el barrio en el vector creado por listToArray e incrementa su campo tree en uno.
int addTree(queryDataADT qd, const char * nbhName, const char * sciName, float diam){
    int result = addScientificTree(qd, sciName, diam);
    if (result == ERROR)
        return ERROR;

    if (qd->arrStatus == UNPREPARED){
        qd->arrStatus = PREPARED;
        if( listToArray(qd) == ERROR)
            return ERROR;
    }
    int index = binSearch( nbhName, qd->nbhArr, qd->arrDim);

    if(index >= 0)
        (qd->nbhArr[index].trees)++;
    return SUCCESS;
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

/* Ordena el vector siguiendo un criterio determinado de acuerdo a la query que
* se desee (pasandole el mismo como parametro utilizando un puntero a funcion)
*/
static nbhList arrayToList(nbhInfo * arr, size_t size, pCompFunc criteria) {
  int ok;
  nbhList first = NULL;
  for (size_t i = 0; i < size; i++) {
    ok = !SUCCESS;
    first = addNbhRec(first, arr + i, &ok, criteria);
    if (ok == ERROR) {
        freeList(first);
        return NULL;
    }
  }
  return first;
}

void toBegin(queryDataADT qd){
  qd->current = qd->firstNbh;
  qd->itSpecies = 0;
}

int beginQuery(queryDataADT qd, int queryNum){
    static pCompFunc compArr[MAX_QUERY] = {compareQ1, compareQ2, compareQ3};
    if(queryNum <= 0 || queryNum > MAX_QUERY)
        return ERROR;

    qd->currQuery = queryNum;
    switch(queryNum) {
        case 1:
        case 2:
            freeList(qd->firstNbh);
            // Crear la lista ordenada de acuerdo al criterio de la query1
            qd->firstNbh = arrayToList(qd->nbhArr, qd->listDim, compArr[queryNum-1]);
            if(qd->firstNbh == NULL)
                return ERROR;
            // Setear iterador a head
            toBegin(qd);
            return SUCCESS;
        case 3:
            qsort(qd->speciesArr, qd->speciesCount, sizeof(struct treeSpecie), compArr[queryNum-1]);
            qd->itSpecies = 0;
            #if DEBUG
                printf("%lu\n", qd->speciesCount);
            #endif
            return SUCCESS;
        default:
            return ERROR;

    }
}

int hasNext(queryDataADT qd) {
    switch(qd->currQuery) {
        case 1:
        case 2:
            return qd->current != NULL;
        case 3:
            return qd->itSpecies != qd->speciesCount;
        default:
            return ERROR;
    }
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
      qd->current = qd->current->tail;
      break;
    case 2:
      // BARRIO;CANT_ARBOLES/HABITANTES
      ans = infoQ2(qd->current->info);
      *size = 2;
      qd->current = qd->current->tail;
      break;
    case 3:
      // NOMBRE_CIENTIFICO;DIAM_PROMEDIO
      ans = infoQ3(qd->speciesArr[qd->itSpecies]);
      *size = 2;
      qd->itSpecies++;
      break;
    default:
      return NULL;
  }

  return ans;
}

static void freeNbhArr(nbhArr arr, size_t size){
  for (size_t i = 0; i < size; i++)
    free(arr[i].name);
  free(arr);
}

static void freeSpeciesArr(treeArr arr, size_t size){
  for (size_t i = 0; i < size; i++)
    free(arr[i].name);
  free(arr);
}

void freeQueryData(queryDataADT qd) {
  freeList(qd->firstNbh);
  freeNbhArr(qd->nbhArr, qd->arrDim);
  freeSpeciesArr(qd->speciesArr, qd->speciesCount);
  free(qd);
}

// qsort(arr, sizeArr, sizeof(struct treeSpecie), cmpSpecie);
