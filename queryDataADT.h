#ifndef QUERY_DATA_ADT_H
#define QUERY_DATA_ADT_H

typedef struct queryDataCDT * queryDataADT;

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
void addTree(queryDataADT qd, const char * nbhName);

/* Función: Libera la memoria almacenada para el TAD
*  Uso: freeQueryData(qd);
*  Descripción: -
*/
void freeQueryData(queryDataADT qd);

/* Función: Prepara los datos según el criterio de la query 1
*  Uso: beginQuery1(qd);
*  Descripción:
*/
void beginQuery1(queryDataADT qd);

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

#endif
