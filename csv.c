#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "csv.h"

#define TRUE_DEL DELIMITER "\n"

char ** readCSVColumns(char * line, const size_t * desiredColumns, size_t quantity) {
    char ** ans = calloc(quantity, sizeof(ans[0]));
    errno = 0;
    if(ans == NULL || errno == ENOMEM){
        perror("Error en calloc()");
        return NULL;
    }

    char * token = strtok(line, TRUE_DEL); // no verificamos token, asumimos formato correcto.

    size_t currField = 0, currCol = 0;
    do {
        if (desiredColumns[currField] == currCol)
          ans[currField++] = token;

        currCol++;
    } while(currField < quantity && (token = strtok(NULL, TRUE_DEL)) != NULL);  // no verificamos token, asumimos formato correcto.

    return ans;
}

void writeCSVLine(FILE * file, char ** arr, size_t size) {
    for (size_t i = 0; i < size; i++)
        fprintf(file, "%s%s", (i == 0 ? "" : DELIMITER), arr[i]);
    fputc('\n', file);
}
