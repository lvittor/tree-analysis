#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "queryDataADT.h"

#define fabs(n) ((n) < 0 ? -(n) : (n))

// Trunca un numero real a 2 decimales.
static float truncate(float value){
    return (int)(value * 100) / 100.0;
}

static char * strDuplicate(const char * src) {
    errno = 0;
    char * dst = malloc(strlen(src) + 1);     // Guarda espacio para el nuevo string.
    if(dst == NULL || errno == ENOMEM) {      // Si no hay memoria
        perror("Error en malloc()");          // error.
        return NULL;
    }                                         // Si no,
    strcpy(dst, src);                         // hace la copia
    return dst;                               // Retorna el nuevo string
}

static char * allocStringFromSize(size_t n) {
    size_t len = snprintf (NULL, 0, "%lu", n);       // Devuelve la cantidad de caracteres del numero
    errno = 0;
    char * ans = malloc((len + 1) * sizeof(ans[0])); // Guarda espacio para el nuevo string
    if(ans == NULL || errno == ENOMEM) {             // Si no hay memoria,
        perror("Error en malloc()");                 // error.
        return NULL;
    }                                                // Si no,
    snprintf(ans, len + 1, "%lu", n);                // Guarda como string el valor de n
    return ans;                                      // Retorna el nuevo string
}

static char * allocStringFromFloat(float f) {
    f = truncate(f);                                  // Trunca a dos decimales
    size_t len = snprintf (NULL, 0, "%.2f", f);       // Devuelve la cantidad de caracteres del numero ya truncado
    errno = 0;
    char * ans = malloc((len + 1) * sizeof(ans[0]));  // Guarda espacio para el nuevo string
    if(ans == NULL || errno == ENOMEM) {               // Si no hay memoria,
        perror("Error en malloc()");                  // error.
        return NULL;
    }                                              // Si no,
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

/* Estructura que guarda la información necesaria para Q3
*/
typedef struct treeSpecie {
    char * name;
    float sumDiam;
    size_t count;
} treeSpecie;

enum {FALSE = 0, TRUE = 1};

/* Arreglo de información de barrios
*  Se ordena para agregar árboles con binSearch
*  Se ordena otra vez según las queries 1 y 2
*/
typedef struct nbhSortedArr{
    nbhInfo * arr;   // Arreglo de barrios ordenados alfabeticamente
    size_t dim;      // Dimensión del arreglo
    char isSorted;   // vale TRUE o FALSE
}nbhSortedArr;

/* Arreglo de informacion de arboles
*  Se ordena segun el criterio de la query 3
*/
typedef struct speciesArr{
    treeSpecie * arr;
    size_t dim;
}speciesArr;

/* Estructura para responder las queries
*/
typedef struct queryDataCDT {
    char currQuery;     // Numero de query actual
    struct nbhSortedArr q1_2;
    struct speciesArr q3;
    size_t iterQuery;
} queryDataCDT;

queryDataADT newQueryData(void){
  errno = 0;
  queryDataADT qd = calloc(1, sizeof(queryDataCDT));
  if(qd == NULL || errno == ENOMEM) {
    perror("Error en calloc()");
    return NULL;
  }
  return qd;
}

/* --- Queries --- */
typedef int (*pCompFunc)(const void *, const void *);
typedef char ** (*pInfoQuery) (const void *, size_t *);

// Query 1
// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES}
// Se debe ordenar de manera decreciente por la cantidad de arboles.
// Si son iguales, se prioriza el orden alfabetico de los barrios.

static int compareQ1(const void * i1, const void * i2){
    struct nbhInfo * info1 = (struct nbhInfo *)i1;
    struct nbhInfo * info2 = (struct nbhInfo *)i2;
    int c = info2->trees - info1->trees;
    if (c != 0)
        return c;
    return strcmp( info1->name, info2->name);
}

static char ** validateStrVec(char ** arr, size_t size) {
    if (arr == NULL)
        return NULL;
    //Doble for pero el tamaño esperado es size = 2
    for (size_t i = 0; i < size; i++){
        if (arr[i] == NULL) {
            for (size_t j = 0; j < size; j++)
                free(arr[j]);
            free(arr);
            return NULL;
        }
    }
    return arr;
}

static char ** infoQ1(const void * s, size_t * size) {
    struct nbhInfo * pNbh = (struct nbhInfo *)s;
    *size = 2;
    errno = 0;
    char ** ans = malloc( (*size) * sizeof(ans[0]) );
    if(ans == NULL || errno == ENOMEM) {
        perror("Error en malloc()");
        return NULL;
    }
    ans[0] = strDuplicate(pNbh->name);
    ans[1] = allocStringFromSize(pNbh->trees);
    return validateStrVec(ans, *size);
}

// Query 2
// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES/CANT_DE_HABITANTES}
// Se debe ordenar de manera decreciente por la cantidad de arboles.
// Si son iguales, se prioriza el orden alfabetico de los barrios.
static int compareQ2(const void * i1, const void * i2){
    struct nbhInfo * info1 = (struct nbhInfo *)i1;
    struct nbhInfo * info2 = (struct nbhInfo *)i2;
    float ratio1 = (float)info1->trees/info1->population;
    float ratio2 = (float)info2->trees/info2->population;
    float c = truncate(ratio2) - truncate(ratio1);
    if(fabs(c) < EPSILON)
        return strcmp(info1->name, info2->name);
    return (c > 0) ? 1 : -1;
}

// ->{NOMBRE_BARRIO, CANT_DE_ARBOLES/CANT_DE_HABITANTES}
static char ** infoQ2(const void * s, size_t * size) {
    struct nbhInfo * pNbh = (struct nbhInfo *)s;
    *size = 2;
    errno = 0;
    char ** ans = malloc( (*size) * sizeof(ans[0]) );
    if(ans == NULL || errno == ENOMEM) {
        perror("Error en malloc()");
        return NULL;
    }
    ans[0] = strDuplicate(pNbh->name);
    #if DEBUG
        printf("Valor original ratio arbol/persona: %f\n", (float)pNbh->trees/pNbh->population);
    #endif
    ans[1] = allocStringFromFloat((float)pNbh->trees/pNbh->population);
    return validateStrVec(ans, *size);
}

static int compareQ3(const void * s1, const void * s2) {
    struct treeSpecie * specie1 = (struct treeSpecie *)s1;
    struct treeSpecie * specie2 = (struct treeSpecie *)s2;
    float avDiam1 = specie1->sumDiam/specie1->count;
    float avDiam2 = specie2->sumDiam/specie2->count;
    float c = truncate(avDiam2) - truncate(avDiam1);
    if (fabs(c) < EPSILON)
        return strcmp(specie1->name, specie2->name);
    return (c < 0) ? -1 : 1;
}

// ->{NOMBRE_CIENTIFICO_ARBOL, DIAM_PROMEDIO}
static char ** infoQ3(const void * s, size_t * size) {
    struct treeSpecie * pTree = (struct treeSpecie *)s;
    *size = 2;
    errno = 0;
    char ** ans = malloc( (*size) * sizeof(ans[0]) );
    if(ans == NULL || errno == ENOMEM) {
        perror("Error en malloc()");
        return NULL;
    }
    ans[0] = strDuplicate(pTree->name);
    #if DEBUG
        printf("Valor original promedio diam: %f\n", pTree->sumDiam/pTree->count);
    #endif
    ans[1] = allocStringFromFloat(pTree->sumDiam/pTree->count);
    return validateStrVec(ans, *size);
}

// Se utiliza en addNbh para dar orden alfabetico a los barrios
static int compareByName(const void * i1, const void * i2){
  struct nbhInfo * info1 = (struct nbhInfo *)i1;
  struct nbhInfo * info2 = (struct nbhInfo *)i2;
  return strcmp(info1->name, info2->name);
}

int addNbh(queryDataADT qd, char * name, size_t population){
    for (size_t i = 0; i < qd->q1_2.dim; i++) {
        if(strcmp(name, qd->q1_2.arr[i].name) == 0){
            fprintf(stderr, "Error al agregar el barrio %s : ya existe.", name);
            return ERROR;
        }
    }
    errno = 0;
    nbhInfo * tmp = realloc(qd->q1_2.arr, (qd->q1_2.dim + 1) * sizeof(tmp[0]));
    if (tmp == NULL || errno == ENOMEM) {
        perror("Error en realloc()");
        return ERROR;
    }
    qd->q1_2.arr = tmp;
    size_t lastIndex = (qd->q1_2.dim)++; // Guardamos el último índice y aumentamos la dimensión en 1.
    if((qd->q1_2.arr[lastIndex].name = strDuplicate(name)) == NULL)
        return ERROR;
    qd->q1_2.arr[lastIndex].population = population;
    qd->q1_2.arr[lastIndex].trees = 0;
    qd->q1_2.isSorted = FALSE;
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
    for (size_t i = 0; i < qd->q3.dim; i++) {
        if (strcmp(speciesName, qd->q3.arr[i].name) == 0) {   // Si la especia ya estaba guardada,
            qd->q3.arr[i].count++;                            // Aumenta la cantidad
            qd->q3.arr[i].sumDiam += diam;                    // Actualiza sumatoria de diametros
            return SUCCESS;
        }
    }

    /* --- Si no todavía no se agregó la especie "speciesName" --- */
    errno = 0;
    treeSpecie * tmp = realloc(qd->q3.arr, (qd->q3.dim + 1) * sizeof(tmp[0]));
    if (tmp == NULL || errno == ENOMEM) {
        perror("Error en realloc()");
        return ERROR;
    }
    qd->q3.arr = tmp;
    size_t lastIndex = (qd->q3.dim)++;
    if((qd->q3.arr[lastIndex].name = strDuplicate(speciesName)) == NULL)
        return ERROR;
    qd->q3.arr[lastIndex].count = 1;
    qd->q3.arr[lastIndex].sumDiam = diam;
    return SUCCESS;
}

static int addTreeToNbh(queryDataADT qd, const char * nbhName){
    if (qd->q1_2.isSorted == FALSE) {
        qsort(qd->q1_2.arr, qd->q1_2.dim, sizeof(qd->q1_2.arr[0]), compareByName); // Ordenar por nombre
        qd->q1_2.isSorted = TRUE;
    }

    int index = binSearch(nbhName, qd->q1_2.arr, qd->q1_2.dim);
    if(index < 0){
        fprintf(stderr, "Error al agregar árbol: %s no existe.", nbhName);
        return ERROR;
    }
    (qd->q1_2.arr[index].trees)++;
    return SUCCESS;
}

int addTree(queryDataADT qd, const char * nbhName, const char * sciName, float diam){
    if (addScientificTree(qd, sciName, diam) == ERROR || addTreeToNbh(qd, nbhName) == ERROR)
        return ERROR;
    return SUCCESS;
}

int beginQuery(queryDataADT qd, int queryNum){
    static pCompFunc compArr[QUERIES] = {compareQ1, compareQ2, compareQ3};

    if(queryNum <= 0 || queryNum > QUERIES){
        fprintf(stderr, "Error al hacer la query %d: no existe.", queryNum);
        return ERROR;
    }

    qd->iterQuery = 0;
    qd->currQuery = queryNum;
    switch(qd->currQuery) {
        case 1: case 2:
            qsort(qd->q1_2.arr, qd->q1_2.dim, sizeof(qd->q1_2.arr[0]), compArr[qd->currQuery-1]);
            break;
        case 3:
            qsort(qd->q3.arr, qd->q3.dim, sizeof(qd->q3.arr[0]), compArr[qd->currQuery-1]);
            #if DEBUG
                printf("Cantidad de especies encontradas: %lu\n\n", qd->q3.dim);
            #endif
            break;
    }

    return SUCCESS;
}

int hasNext(queryDataADT qd) {
    switch(qd->currQuery) {
        case 1: case 2:
            return qd->iterQuery < qd->q1_2.dim;
        case 3:
            return qd->iterQuery < qd->q3.dim;
    }
    return 0;
}

char ** answer(queryDataADT qd, size_t * size) {
    static pInfoQuery getInfo[QUERIES] = {infoQ1, infoQ2, infoQ3};

    if(! hasNext(qd)){
        fprintf(stderr, "No hay datos para acceder.\n");
        return NULL;
    }

    char ** ans;
    switch(qd->currQuery) {
        case 1: case 2:
            //BARRIO;CANT_ARBOLES o BARRIO;CANT_ARBOLES/HABITANTES
            ans = getInfo[qd->currQuery - 1](&(qd->q1_2.arr[qd->iterQuery]), size);
            break;
        case 3:
            // NOMBRE_CIENTIFICO;DIAM_PROMEDIO
            ans = getInfo[qd->currQuery - 1](&(qd->q3.arr[qd->iterQuery]), size);
            break;
    }
    if(ans != NULL) // si se pudo obtener informacion de la query
        (qd->iterQuery)++; 
    
    return ans;
}

static void freeNbhArr(nbhInfo * arr, size_t size){
  for (size_t i = 0; i < size; i++)
    free(arr[i].name);
  free(arr);
}

static void freeSpeciesArr(treeSpecie * arr, size_t size){
  for (size_t i = 0; i < size; i++)
    free(arr[i].name);
  free(arr);
}

void freeQueryData(queryDataADT qd) {
  freeNbhArr(qd->q1_2.arr, qd->q1_2.dim);
  freeSpeciesArr(qd->q3.arr, qd->q3.dim);
  free(qd);
}
