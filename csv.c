#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"


static char * strDuplicate(const char * src) {
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    if (dst == NULL)                        // Si no hay memoria,
        perror("Error en malloc()");        // error.
    else                                    // Si no,
        strcpy(dst, src);                   // hace la copia
    return dst;                             // Retorna el nuevo string
}

char ** getColumns(const char * line, const size_t * desiredColumns, size_t quantity) {
    char ** ans = calloc(quantity, sizeof(ans[0]));
    if(ans == NULL){
        perror("Error en calloc()");
        return NULL;
    }

    char * tmp = strDuplicate(line);
    if(tmp == NULL)
        return NULL;

    char * token = strtok(tmp, DELIMITER); // no verificamos token, asumimos formato correcto.

    size_t currField = 0, currCol = 0;
    do {
        if (desiredColumns[currField] == currCol){
          char * aux = strDuplicate(token);
          if(aux == NULL)
              return NULL;
          ans[currField++] = aux;
        }
        currCol++;
    } while(currField < quantity && (token = strtok(NULL, DELIMITER)) != NULL);  // no verificamos token, asumimos formato correcto.

    free(tmp);

    return ans;
}
