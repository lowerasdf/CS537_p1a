#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void main(int argc, char *argv[]) {
    // read db
    FILE *file = fopen("database.txt", "r");
    assert(file != NULL);

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while((linelen = getline(&line, &linecap, file)) > 0) {
        printf("input: %s\n", line);
    }

    for(int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);

        char *token, *string, *tofree;

        tofree = string = strdup(argv[i]);
        assert(string != NULL);

        while((token = strsep(&string, ",")) != NULL) {
            printf("  token:%s\n", token);

            // check validity
        }

        free(tofree)

        // if valid do something
    }

    // store db
}