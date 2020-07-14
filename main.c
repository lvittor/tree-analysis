#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // VER SI HACE FALTA ACA O PONERLO EN qdADT.h
#include "csv.h"
#include "queryDataADT.h"

#define MAX_OUTPUT_NAME 15 // Tamaño maximo del nombre del archivo de salida
#define MAX_BUFF 1024    // Tamaño maximo del buffer de lectura

#define RETURN_ERROR    1

#define FIELDS_TREE 3 // Cantidad de campos relevantes a leer de arboles.csv

/* Número de columna de Comuna, Nombre cientifico, Diametro en arboles.csv
*  Necesariamente de menor a mayor
*/
#ifdef BUE
    size_t treePos[FIELDS_TREE] = {2, 7, 11};
    enum {TREE_NBH = 0, TREE_SCI_NAME, TREE_DIAM};
#elif VAN
    size_t treePos[FIELDS_TREE] = {6, 12, 15};
    enum {TREE_SCI_NAME = 0, TREE_NBH, TREE_DIAM};
#endif


#define FIELDS_NBH  2 // Cantidad de campos relevantes a leer de barrio.csv

/* Número de columna de Nombre, Cantidad de habitantes en barrio.csv */
size_t nbhPos[FIELDS_NBH] = {0, 1};
enum {NBH_NAME = 0, NBH_POP};

/* Encabazados de los archivos de salida */
const char * headers[QUERIES] = {
    "BARRIO;ARBOLES",
    "BARRIO;ARBOLES_POR_HABITANTE",
    "NOMBRE_CIENTIFICO;PROMEDIO_DIAMETRO"
};

/* Función: Imprimime un vector de punteros a char mediante salida estandar */
void printArrayOfStrings(char ** strArr, size_t size);

/* Función: Permite abrir liberar la memoria del vector de strings generado por getColumns */
void freeVec(char ** vec, size_t size);

/* Función: Libera memoria alocada y cierra archivos cuando se quiere salir del programa */
int safeExit(queryDataADT qd, FILE * file);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Debe pasar exactamente 2 argumentos.\n");
        return RETURN_ERROR;
    }

    char * treeCSVPath = argv[1];
    char * nbhCSVPath  = argv[2];

    char line[MAX_BUFF];
    FILE * file;
    queryDataADT qd = newQueryData();
    if (qd == NULL)
        return RETURN_ERROR;

    /* --- Lectura de datos de barrios.csv ---*/

    if((file = fopen(nbhCSVPath, "r")) == NULL){
        perror("Error leyendo el archivo de barrios");
        freeQueryData(qd);
        return RETURN_ERROR;
    }

    fgets(line, MAX_BUFF, file); // remueve el header del .csv
    while(fgets(line, MAX_BUFF, file) != NULL) {
        char ** rowData = readCSVColumns(line, nbhPos, FIELDS_NBH);
        #if DEBUG
            printf("Se leyo el barrio:\n");
            printArrayOfStrings(rowData, FIELDS_NBH);
        #endif
        if (addNbh(qd, rowData[NBH_NAME], atoi(rowData[NBH_POP])) == ERROR)
            return safeExit(qd, file);
        free(rowData);
    }

    fclose(file);

    /* --- Lectura de datos de arboles.csv --- */

    if((file = fopen(treeCSVPath, "r")) == NULL){
        perror("Error leyendo el archivo de arboles");
        freeQueryData(qd);
        return RETURN_ERROR;
    }

    fgets(line, MAX_BUFF, file); // remueve el header del .csv
    while(fgets(line, MAX_BUFF, file) != NULL) {
        char ** rowData = readCSVColumns(line, treePos, FIELDS_TREE);
        #if DEBUG
            printf("Se leyo el arbol:\n");
            printArrayOfStrings(rowData, FIELDS_TREE);
        #endif
        if (addTree(qd, rowData[TREE_NBH], rowData[TREE_SCI_NAME], atof(rowData[TREE_DIAM])) == ERROR)
            return safeExit(qd, file);
        free(rowData);
    }

    fclose(file);

    for (unsigned int i = 1; i <= QUERIES; i++) {
        #if DEBUG
            printf("\n--------- QUERY %d ---------\n", i);
        #endif

        char outputName[MAX_OUTPUT_NAME];
        snprintf(outputName, MAX_OUTPUT_NAME, "query%d.csv", i);

        if ( (file = fopen(outputName, "w")) == NULL) {
            perror("Error creando el archivo de salida");
            return RETURN_ERROR;
        }

        if(beginQuery(qd, i) == ERROR)
            return safeExit(qd, file);

        fprintf(file, "%s\n", headers[i-1]);

        size_t size;
        while(hasNext(qd)) {
            char ** ans = answer(qd, &size);
            if (ans == NULL)
                return safeExit(qd, file);

            #if DEBUG
                printArrayOfStrings(ans, size);
            #endif
            writeCSVLine(file, ans, size);
            freeVec(ans, size);
        }
        fclose(file);
    }

    freeQueryData(qd);

    return 0;
}

void printArrayOfStrings(char ** strArr, size_t size) {
    for (int i = 0; i < size; i++)
        printf("%s\n", strArr[i]);
    putchar('\n');
}

void freeVec(char ** vec, size_t size){
  for(int i = 0; i < size; i++)
      free(vec[i]);
  free(vec);
}

int safeExit(queryDataADT qd, FILE * file) {
    if (qd != NULL)
        freeQueryData(qd);
    if (file != NULL)
        fclose(file);
    return RETURN_ERROR;
}
