#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const char *db_name = "database.txt";

int main(int argc, char *argv[]) {
    // TODO hash table
    
    // read db
    FILE *file = fopen(db_name, "r");
    if (file != NULL) {
        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;
        while((linelen = getline(&line, &linecap, file)) > 0) {
            printf("input: %s", line);
        }
        
        // TODO add to hash table

        fclose(file);
    }

    for(int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);

        char *token, *string, *tofree;

        tofree = string = strdup(argv[i]);
        assert(string != NULL);

        while((token = strsep(&string, ",")) != NULL) {
            printf("  token:%s\n", token);

            // TODO check validity
        }

        free(tofree);

        // TODO if valid do something
    }

    // TODO store db
    FILE *output_file = fopen(db_name, "w");
    assert(output_file != NULL);
}