#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // VER SI HACE FALTA ACA O PONERLO EN qdADT.h
#include "csv.h"
#include "queryDataADT.h"

#define MAX_BUFF  1024 // Tamaño maximo del buffer de lectura
#define FIELDS_TREE 3 // Cantidad de campos relevantes a leer de arboles.csv


/* Número de columna de Comuna, Nombre cientifico, Diametro en arboles.csv
*  Necesariamente de menor a mayor
*/
#ifdef BUE  // Determina si esta en modo BUE o VAN
    size_t treePos[FIELDS_TREE] = {2, 7, 11};
    enum {TREE_NBH = 0, TREE_SCI_NAME, TREE_DIAM};
#else
    size_t treePos[FIELDS_TREE] = {6, 12, 15};
    enum {TREE_SCI_NAME = 0, TREE_NBH, TREE_DIAM};
#endif


#define FIELDS_NBH  2 // Cantidad de campos relevantes a leer de barrio.csv

/* Número de columna de Nombre, Cantidad de habitantes en barrio.csv */
size_t nbhPos[FIELDS_NBH] = {0, 1};
enum {NBH_NAME = 0, NBH_POP};

/* Función: Imprimime un vector de punteros a char mediante salida estandar */
void printArrayOfStrings(char ** strArr, size_t size);

/* Función: Permite abrir liberar la memoria del vector de strings generado por getColumns */
void freeVec(char ** vec, size_t size);

int main(int argc, char *argv[]) {
    char line[MAX_BUFF];
    FILE * file;
    queryDataADT qd = newQueryData();

    file = fopen(argv[2], "r");
    if(file == NULL){
        perror("Error leyendo el archivo de barrios");
        return 1;
    }

    fgets(line, MAX_BUFF, file); // remueve el header del .csv
    while(fgets(line, MAX_BUFF, file) != NULL) {
        char ** rowData = getColumns(line, nbhPos, FIELDS_NBH);
        //printArrayOfStrings(rowData, FIELDS_NBH);
        addNbh(qd, rowData[NBH_NAME], atoi(rowData[NBH_POP])); // Buscar funcion string to unsigned
        freeVec(rowData, FIELDS_NBH);
    }

    fclose(file);

    #if DEBUG
        toBegin(qd);
        while(hasNext(qd)) {
            char ** namePop = next(qd); //char ** next(queryDataADT qd);
            printArrayOfStrings(namePop, FIELDS_NBH);
            freeVec(namePop, FIELDS_NBH);
        }
    #endif

    #if DEBUG
        printf("\n--------------------------------------------------------\n");
    #endif

    file = fopen(argv[1], "r");
    if(file == NULL){
        perror("Error leyendo el archivo de arboles");
        return 1;
    }

    while(fgets(line, MAX_BUFF, file) != NULL) {
        char ** rowData = getColumns(line, treePos, FIELDS_TREE);
        // printArrayOfStrings(rowData, FIELDS_TREE);
        addTree(qd, rowData[TREE_NBH]);
        freeVec(rowData, FIELDS_TREE);
    }

    fclose(file);

    if(beginQuery1(qd) == ERROR)
      return 1;

    size_t size;
    while(hasNext(qd)) {
        char ** ans = answer(qd, &size);
        #if DEBUG
            printArrayOfStrings(ans, size);
        #endif
        freeVec(ans, size);
    }

    #if DEBUG
        printf("\n--------------------------------------------------------\n");
    #endif

    if(beginQuery2(qd) == ERROR)
      return 1;

    while(hasNext(qd)) {
        char ** ans = answer(qd, &size);
        #if DEBUG
            printArrayOfStrings(ans, size);
        #endif
        freeVec(ans, size);
    }

    freeQueryData(qd);

    return 0;
}

void printArrayOfStrings(char ** strArr, size_t size) {
    for (int i = 0; i < size; i++)
        printf("%d - %s\n", i, strArr[i]);
    putchar('\n');
}

void freeVec(char ** vec, size_t size){
  for(int i = 0; i < size; i++)
      free(vec[i]);
  free(vec);
}
