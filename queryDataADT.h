#ifndef QUERY_DATA_ADT_H
#define QUERY_DATA_ADT_H
#define EPSILON 0.00001    // Util para la Query 2
#define ERROR   0          // hubo error de memoria
#define SUCCESS !ERROR     // no hubo error de memoria

typedef struct queryDataCDT * queryDataADT;

#define QUERIES 3

/* Función: Crea una nueva instancia de queryDataADT
*  Uso: queryDataADT qd = newQueryData(void);
*  Descripción: -
*/
queryDataADT newQueryData(void);

/* Función: Almacena la información del barrio
*  Uso: addNbh(qd, name, population);
*  Descripción: Agrega un barrio de nombre "name" y su cantidad de habitantes "population".
*/
int addNbh(queryDataADT qd, char * name, size_t population);

/* Función: Almacena la información del arbol
*  Uso: addTree(queryDataADT qd, const char * nbhName);
*  Descripción: Agrega un árbol cuyo barrio es nbhName, cuyo nombre cientifico es sciName y cuyo diametro a la altura del pecho es diam.
*/
int addTree(queryDataADT qd, const char * nbhName, const char * sciName, float diam);

/* Función: Libera la memoria almacenada para el TAD
*  Uso: freeQueryData(qd);
*  Descripción: -
*/
void freeQueryData(queryDataADT qd);

/* Función: Prepara el recorrido de las respuestas para una query particular.
*  Uso: beginQuery(qd, queryNum);
*  Descripción: Retorna ERROR si ocurrió un fallo y no se puede comenzar la query.
*  Retorna SUCCESS si es posible comenzar.
*  queryNum debe encontrarse entre 1 y QUERIES inclusive.
*/
int beginQuery(queryDataADT qd, int queryNum);

/* Función: Permite recorrer los datos hasta que se terminen.
*  Uso: hasNext(qd)
*  Descripción: Devuelve 1 si hay mas datos, 0 sino.
*/
int hasNext(queryDataADT qd);

/* Función: Retorna uno a uno los datos correspondientes a cada query
*  Uso: char ** ans = answer(qd, &size);
*  Descripción: Devuelve un vector de punteros a char correspondientes a la query actual.
*  Se debe liberar la memoria almacenada para cada string.
*/
char ** answer(queryDataADT qd, size_t * size);

#endif
