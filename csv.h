/* Libreria para leer archivos csv
*  1. Se lo puede recorrer hacia adelante
*  2. En cada linea se puede preguntar por columnas especificas
*/

#ifndef CSV_H
#define CSV_H

/* Definición del tipo de delimitador */
#define DELIMITER ";"

/* Función: Permite extraer del .csv los campos (columnas) de la linea actual.
*  Uso: rowData = getColumns(line, desiredColumns, quantity)
*  Descripción: Devuelve un vector de punteros a char que contenga los campos (columnas) especificadas en desiredColumns.
*  quantity es la dimension de desiredColumns y este arreglo debe estar ordenado ascendentemente.
*/
void readCSVColumns(char * line, const size_t * desiredColumns, size_t quantity, char * output[]);

/* Función: Escribe en el archivo .csv (file) el arreglo pasado por parametro (arr) con dimension (size).
*  Uso: -
*  Descripción: -
*/
void writeCSVLine(FILE * file, char ** arr, size_t size);

#endif
