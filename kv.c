#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

const char *db_name = "database.txt";
const int save_to_db_when_exit = 1;
const int hash_capacity = 1000;
const int buffer_size = 1000;
const char *arg_put = "p";
const char *arg_get = "g";
const char *arg_delete = "d";
const char *arg_clear = "c";
const char *arg_all = "a";

struct Node {
    int key;
    char *val;
    struct Node *next;
};

void printList(struct Node* root) {
    struct Node *curr = root;
    while (curr != NULL) {
        printf("%d,%s\n", curr->key, curr->val);
        curr = curr->next;
    }
}

void freeNode(struct Node* node) {
    free(node->val);
    free(node);
}

void freeAll(struct Node* node) {
    if (node == NULL) {
        return;
    }
    freeAll(node->next);
    freeNode(node);
}

void appendOrSet(struct Node** root, int new_key, char *new_val) {
    if (root == NULL) {
        printf("appendOrSet: pointer to root should not be null\n");
        return;
    }

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *curr = *root;
    new_node->key = new_key;
    new_node->val = new_val;
    new_node->next = NULL;

    if (*root == NULL) {
        *root = new_node;
        return;
    }

    while (curr->next != NULL) {
        if (curr->key == new_key) {
            free(new_node);
            curr->val = new_val;
            return;
        }
        curr = curr->next;
    }
    if (curr->key == new_key) {
        free(new_node);
        curr->val = new_val;
        return;
    }

    curr->next = new_node;
    return;
}

char* get(struct Node* root, int key) {
    struct Node *curr = root;
    while (curr != NULL) {
        if (curr->key == key) {
            return curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}

int deleteNode(struct Node** root, int key) {
    if (root == NULL) {
        printf("deleteNode: pointer to root should not be null\n");
        return 0;
    }

    struct Node *curr = *root;
    struct Node *prev = *root;
 
    if (curr != NULL && curr->key == key) {
        *root = curr->next;
        freeNode(curr);
        return 1;
    }
 
    while (curr != NULL && curr->key != key) {
        prev = curr;
        curr = curr->next;
    }
 
    if (curr == NULL) {
        return 0;
    }
 
    prev->next = curr->next;
    freeNode(curr);
    return 1;
}

int hash(int key) {
    return key % hash_capacity;
}

int isNumber(char *s) {
    if (s[0] == '\0') {
        return 0;
    }
    for (int i = 0; s[i] != '\0'; i++){
        if (isdigit(s[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    // hash table
    struct Node *hash_table[hash_capacity];
    for(int i = 0; i < hash_capacity; i++) {
        hash_table[i] = NULL;
    }

    // read db
    FILE *file = fopen(db_name, "r");
    if (file != NULL) {
        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;
        while((linelen = getline(&line, &linecap, file)) > 1) {
            char *token;
            int temp = 0;
            int key;
            char *val = malloc((buffer_size + 1) * sizeof(char));
            while((token = strsep(&line, ",")) != NULL) {
                if (temp == 0) {
                    key = atoi(token);
                } else if (temp == 1) {
                    token[strcspn(token, "\n")] = 0;
                    strcpy(val, token);
                }
                temp += 1;
            }

            int hash_code = hash(key);
            appendOrSet(&hash_table[hash_code], key, val);
        }

        fclose(file);
    }

    for(int i = 1; i < argc; i++) {
        char *token, *string, *tofree;
        tofree = string = strdup(argv[i]);
        assert(string != NULL);

        int i = 0;
        int bad_command = 0;
        char **tokens = malloc(3 * sizeof(char*));;
        for(int i = 0; i < 3; i++) {
            tokens[i] = malloc((buffer_size+1) * sizeof(char));
            strcpy(tokens[i], "\0");
        }
        while((token = strsep(&string, ",")) != NULL) {
            if (i >= 3) {
                bad_command = 1;
                break;
            }

            strcpy(tokens[i], token);
            i += 1;
        }

        free(tofree);

        if (bad_command) {
            printf("bad command\n");
            for(int i = 0; i < 3; i++) {
                free(tokens[i]);
            }
            free(tokens);
            continue;
        }

        // p,key,val
        if(strcmp(tokens[0], arg_put) == 0) {
            if (isNumber(tokens[1])) {
                int key = atoi(tokens[1]);
                if (strcmp(tokens[2],"\0") != 0) {
                    int hash_code = hash(key);
                    char* val = strdup(tokens[2]);
                    appendOrSet(&hash_table[hash_code], key, val);
                } else {
                    bad_command = 1;
                }
            } else {
                bad_command = 1;
            }
        } 
        // g,key
        else if(strcmp(tokens[0], arg_get) == 0) {
            if (isNumber(tokens[1])) {
                int key = atoi(tokens[1]);
                if (strcmp(tokens[2],"\0") == 0) {
                    int hash_code = hash(key);
                    char *val = get(hash_table[hash_code], key);
                    if (val != NULL) {
                        printf("%d,%s\n", key, val);
                    } else {
                        printf("%d not found\n", key);
                    }
                } else {
                    bad_command = 1;
                }
            } else {
               bad_command = 1; 
            }
        }
        // d,key
        else if(strcmp(tokens[0], arg_delete) == 0) {
            if (isNumber(tokens[1])) {
                int key = atoi(tokens[1]);
                if (strcmp(tokens[2],"\0") == 0) {
                    int hash_code = hash(key);
                    int found = deleteNode(&hash_table[hash_code], key);
                    if (!found) {
                        printf("%d not found\n", key);
                    }
                } else {
                    bad_command = 1;
                }
            } else {
               bad_command = 1; 
            }
        }
        // c
        else if(strcmp(tokens[0], arg_clear) == 0) {
            if (strcmp(tokens[1],"\0") == 0 && strcmp(tokens[2],"\0") == 0) { 
                for(int i = 0; i < hash_capacity; i++) {
                    freeAll(hash_table[i]);
                    hash_table[i] = NULL;
                }   
            } else {
                bad_command = 1; 
            }
        }
        // a
        else if(strcmp(tokens[0], arg_all) == 0) {
            if (strcmp(tokens[1],"\0") == 0 && strcmp(tokens[2],"\0") == 0) { 
                for(int i = 0; i < hash_capacity; i++) {
                    printList(hash_table[i]);
                }   
            } else {
                bad_command = 1; 
            }
        }
        // invalid command
        else {
            bad_command = 1; 
        }

        for(int i = 0; i < 3; i++) {
            free(tokens[i]);
        }
        free(tokens);

        if (bad_command) {
            printf("bad command\n");
            continue;
        }
    }

    // store db
    if (save_to_db_when_exit) {
        FILE *output_file = fopen(db_name, "w");
        assert(output_file != NULL);

        for(int i = 0; i < hash_capacity; i++) {
            if (hash_table[i] != NULL) {
                struct Node *curr = hash_table[i];
                while(curr != NULL) {
                    fprintf(output_file,"%d,%s\n", curr->key, curr->val);                
                    curr = curr->next;
                }
            }
        }

        fclose(output_file);
    }

    // free hash table
    for(int i = 0; i < hash_capacity; i++) {
        freeAll(hash_table[i]);
    }
}