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
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    errno = 0;
    if(dst == NULL || errno == ENOMEM)      // Si no hay memoria
        perror("Error en malloc()");        // error.
    else                                    // Si no,
        strcpy(dst, src);                   // hace la copia
    return dst;                             // Retorna el nuevo string
}

static char * allocStringFromSize(size_t n) {
    size_t len = snprintf (NULL, 0, "%lu", n);       // Devuelve la cantidad de caracteres del numero
    char * ans = malloc((len + 1) * sizeof(ans[0])); // Guarda espacio para el nuevo string
    errno = 0;
    if(ans == NULL || errno == ENOMEM)               // Si no hay memoria,
        perror("Error en malloc()");                 // error.
    else                                             // Si no,
        snprintf(ans, len + 1, "%lu", n);            // Guarda como string el valor de n
    return ans;                                      // Retorna el nuevo string
}

static char * allocStringFromFloat(float f) {
    f = truncate(f);                                  // Trunca a dos decimales
    size_t len = snprintf (NULL, 0, "%.2f", f);       // Devuelve la cantidad de caracteres del numero ya truncado
    char * ans = malloc((len + 1) * sizeof(ans[0]));  // Guarda espacio para el nuevo string
    errno = 0;
    if(ans == NULL || errno == ENOMEM)                // Si no hay memoria,
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
