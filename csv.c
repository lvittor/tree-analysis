#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "csv.h"


#define TRUE_DEL DELIMITER "\n"

static char * strDuplicate(const char * src) {
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    errno = 0;
    if (dst == NULL || errno == ENOMEM)     // Si no hay memoria,
        perror("Error en malloc()");        // error.
    else                                    // Si no,
        strcpy(dst, src);                   // hace la copia
    return dst;                             // Retorna el nuevo string
}

char ** readCSVColumns(const char * line, const size_t * desiredColumns, size_t quantity) {
    char ** ans = calloc(quantity, sizeof(ans[0]));
    errno = 0;
    if(ans == NULL || errno == ENOMEM){
        perror("Error en calloc()");
        return NULL;
    }

    char * tmp = strDuplicate(line);
    if(tmp == NULL)
        return NULL;

    char * token = strtok(tmp, TRUE_DEL); // no verificamos token, asumimos formato correcto.

    size_t currField = 0, currCol = 0;
    do {
        if (desiredColumns[currField] == currCol){
          char * aux = strDuplicate(token);
          if(aux == NULL)
              return NULL;
          ans[currField++] = aux;
        }
        currCol++;
    } while(currField < quantity && (token = strtok(NULL, TRUE_DEL)) != NULL);  // no verificamos token, asumimos formato correcto.

    free(tmp);

    return ans;
}

void writeCSVLine(FILE * file, char ** arr, size_t size) {
    for (size_t i = 0; i < size; i++)
        fprintf(file, "%s%s", (i == 0 ? "" : DELIMITER), arr[i]);
    fputc('\n', file);
}
