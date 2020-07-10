#ifndef QUERY_DATA_ADT_H
#define QUERY_DATA_ADT_H
#define EPSILON 0.00001    // Util para la Query 2
#define ERROR   0          // hubo error de memoria
#define SUCCESS !ERROR     // no hubo error de memoria

typedef struct queryDataCDT * queryDataADT;

#define QUERIES 3

/* Función: Crea una nueva instancia de queryDataADT
*  Uso: qd = newQueryData(void);
*  Descripción: -
*/
queryDataADT newQueryData(void);

/* Función: Almacena la información del barrio
*  Uso: addNbh(qd, name, population);
*  Descripción: Agrega un barrio de nombre "name" y su cantidad de habitantes "population".
*  Si ya se había agregado el mismo nombre, no hace nada.
*/
void addNbh(queryDataADT qd, char * name, size_t population);

/* Función: Almacena la información del arbol
*  Uso: addTree(queryDataADT qd, const char * nbhName);
*  Descripción: Agrega un árbol cuyo barrio es "nbhName".
*  De no existir el barrio, no hace nada.
*/
int addTree(queryDataADT qd, const char * nbhName, const char * sciName, float diam);

/* Función: Libera la memoria almacenada para el TAD
*  Uso: freeQueryData(qd);
*  Descripción: -
*/
void freeQueryData(queryDataADT qd);

int beginQuery(queryDataADT qd, int queryNum);

/* Función:
*  Uso:
*  Descripción:
*/
int hasNext(queryDataADT qd);

/* Función:
*  Uso:
*  Descripción:
*/
char ** answer(queryDataADT qd, size_t * size);

/* Función:
*  Uso:
*  Descripción:
*/
void toBegin(queryDataADT qd);

/* Función:
*  Uso:
*  Descripción:
*/
#if DEBUG
char ** next(queryDataADT qd);
#endif

#endif
