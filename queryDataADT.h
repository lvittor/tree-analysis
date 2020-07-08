#ifndef QUERY_DATA_ADT_H
#define QUERY_DATA_ADT_H

typedef struct queryDataCDT * queryDataADT;

/* Función: 
*  Uso:    
*  Descripción:
*/
queryDataADT newQueryData(void);

/* Función: 
*  Uso:
*  Descripción:
*/
void addNbh(queryDataADT qd, char * name, size_t population);

/* Función:
*  Uso:
*  Descripción:
*/
void addTree(queryDataADT qd, const char * nbhName);

/* Función:
*  Uso:
*  Descripción:
*/
void freeQueryData(queryDataADT qd);

/* Función:
*  Uso:
*  Descripción:
*/
void toBegin(queryDataADT qd);

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
void beginQuery1(queryDataADT qd);

#endif
