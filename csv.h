/* Libreria para leer archivos csv
*  1. Se lo puede recorrer hacia adelante
*  2. En cada linea se puede preguntar por columnas especificas
*/

#ifndef CSV_H
#define CSV_H

/* Definición del tipo de delimitador */
#define DELIMITER ";\n"

/* Función: Permite abrir el .csv en modo lectura mediante un path al archivo.
*  Uso: file = openCSV(path)
*  Descripción: Retorna un puntero al archivo si se pudo abrir exitosamente, devuelve NULL sino.
*/
FILE * openCSV(const char * path);

/* Función: Permite extraer del .csv los campos (columnas) de la linea actual.
*  Uso: rowData = getColumns(line, desiredColumns, quantity)
*  Descripción: Devuelve un vector de punteros a char que contenga los campos (columnas) especificadas en desiredColumns.
*  quantity es la dimension de desiredColumns y este arreglo debe estar ordenado ascendentemente.
*/
char ** getColumns(const char * line, const size_t * desiredColumns, size_t quantity);

/* Función: Copia en line a lo sumo maxLen - 1 caracteres de file, o hasta encontrar una nueva linea.
*  Uso: nextLine(file, line, maxLen)
*  Descripción: Se utilizará como condición en un ciclo, controlando que al retornar NULL ya no hay lineas por leer en el archivo
*/
char * nextLine(FILE * file, char * line, size_t maxLen);

/* Función: Permite cerrar el file abierto por openCSV
*  Uso: closeCSV(file);
*  Descripción: -
*/
void closeCSV(FILE * file);

#endif
