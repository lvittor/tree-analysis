#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

FILE * openCSV(const char * path) {
    FILE * file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open %s\n", path);
        return NULL;
    }
    return file;
}

static char * strDuplicate(const char * src) {
    char * dst = malloc(strlen(src) + 1);   // Guarda espacio para el nuevo string.
    if (dst != NULL)                        // Si hay memoria,
      strcpy(dst, src);                     // hace la copia
    return dst;                             // Retorna el nuevo string
}

char ** getColumns(const char * line, const size_t * desiredColumns, size_t quantity) {
    char ** ans = calloc(quantity, sizeof(ans[0]));

    char * tmp = strDuplicate(line);
    char * token = strtok(tmp, DELIMITER);

    if (token == NULL) {
        fprintf(stderr, "format error in csv file\n");
        exit(1); // preguntar si hay que hacer exit() o devolver NULL
    }

    size_t currField = 0, currCol = 0;
    do {
        if (desiredColumns[currField] == currCol)
          ans[currField++] = strDuplicate(token);

        currCol++;
    } while(currField < quantity && (token = strtok(NULL, DELIMITER)) != NULL);

    free(tmp);

    return ans;
}

char * nextLine(FILE * file, char * line, size_t maxLen) {
    return fgets(line, maxLen, file);
}

void closeCSV(FILE * file) {
    fclose(file);
}
