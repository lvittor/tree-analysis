#include <stdio.h>
#include <stdlib.h>
#include "csv.h"

#define MAX_BUFF  1024 // Tamaño maximo del buffer de lectura
#define FIELDS_TREE 3 // Cantidad de campos relevantes a leer de arboles.csv

#define BUE "ON"

/* Número de columna de Comuna, Nombre cientifico, Diametro en arboles.csv
*  Necesariamente de menor a mayor
*/
#ifdef BUE  // Determina si esta en modo BUE o VAN
    size_t treePos[FIELDS_TREE] = {2, 7, 11};
    enum {NBH = 0, SCI_NAME, DIAM};
#else
    size_t treePos[FIELDS_TREE] = {6, 12, 15};
    enum {SCI_NAME = 0, NBH, DIAM};
#endif


#define NBH_FIELDS  2 // Cantidad de campos relevantes a leer de barrio.csv

/* Número de columna de Nombre, Cantidad de habitantes en barrio.csv */
size_t nbhPos[NBH_FIELDS] = {0, 1};
enum {NBH_NAME = 0, NBH_POP};

/* Función: Imprimime un vector de punteros a char mediante salida estandar */
void printArrayOfStrings(char ** strArr, size_t size);

/* Función: Permite abrir liberar la memoria del vector de strings generado por getColumns */
void freeVec(char ** vec);

int main(int argc, char *argv[]) {
    FILE * file = openCSV(argv[1]);

    char line[MAX_BUFF];

    while(nextLine(file, line, MAX_BUFF) != NULL) {
        char ** rowData = getColumns(line, treePos, FIELDS_TREE);
        printArrayOfStrings(rowData, FIELDS_TREE);
        // addTree(rowData[NBH], rowData[SCI_NAME], atof(rowData[DIAM]));
        freeVec(rowData);
    }

    closeCSV(file);
    return 0;
}

void printArrayOfStrings(char ** strArr, size_t size) {
    for (int i = 0; i < size; i++)
        printf("%d - %s\n", i, strArr[i]);
    putchar('\n');
}

void freeVec(char ** vec){
  for(int i=0; i<FIELDS_TREE; i++)
      free(vec[i]);
  free(vec);
}
