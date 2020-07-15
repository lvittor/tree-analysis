/* Libreria para leer archivos csv
*  1. Se lo puede recorrer hacia adelante
*  2. En cada linea se puede preguntar por columnas especificas
*/

#ifndef CSV_H
#define CSV_H

/* Definición del tipo de delimitador */
#define DELIMITER ";"

/* Función: Permite extraer del .csv los campos (columnas) de la linea actual.
*  Uso: getColumns(line, desiredColumns, quantity, output)
*  Descripción: Guarda en output punteros a char de las direcciones en line
*  donde comienzan los campos (columnas) especificados en desiredColumns.
*  quantity debe ser la dimensión de desiredColumns y de output.
*  desiredColumns debe estar ordenado ascendentemente.
*  line debe terminar en '\n'.
*  Se reemplazan las apariciones de DELIMITER y el '\n' por '\0'.
*/
void readCSVColumns(char * line, const size_t * desiredColumns, size_t quantity, char * output[]);

/* Función: Escribe en el archivo .csv (file) el arreglo pasado por parametro (arr) con dimension (size).
*  Uso: -
*  Descripción: Interpreta el arreglo como uno de strings.
*  Escribe en file cada string del arreglo separado por DELIMITER y terminando con '\n'.
*/
void writeCSVLine(FILE * file, char ** arr, size_t size);

#endif
