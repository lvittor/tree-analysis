#ifndef QUERY_DATA_ADT_H
#define QUERY_DATA_ADT_H
#define EPSILON 0.00001    // Util para la Query 2
enum {ERROR=0, SUCCESS, ALLOC_ERROR, REPEATED_ERROR, NO_QUERY_VALUE, NO_INDEX};

typedef struct queryDataCDT * queryDataADT;

#define QUERIES 3

/* Función: Crea una nueva instancia de queryDataADT
*  Uso: queryDataADT qd = newQueryData(void);
*  Descripción: Devuelve NULL si no se pudo crear espacio para almacenar los datos
*/
queryDataADT newQueryData(void);

/* Función: Almacena la información del barrio
*  Uso: addNbh(qd, name, population);
*  Descripción: Agrega un barrio de nombre "name" y su cantidad de habitantes "population".
*  Retorna ALLOC_ERROR si no pudo agregar el barrio o REPEATED_ERROR si ya existía.
*/
int addNbh(queryDataADT qd, char * name, size_t population);

/* Función: Almacena la información del arbol
*  Uso: addTree(queryDataADT qd, const char * nbhName);
*  Descripción: Agrega un árbol cuyo barrio es nbhName, cuyo nombre cientifico es sciName y cuyo diametro a la altura del pecho es diam.
*  Retorna ERROR si no pudo agregar el arbol.
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
*  Retorna SUCCESS si es posible comenzar, o NO_QUERY_VALUE si no existe esa query.
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
*  Retorna NULL si no se pudo obtener la respuesta.
*/
char ** answer(queryDataADT qd, size_t * size);

#endif
