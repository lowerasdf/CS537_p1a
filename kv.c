#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const char *db_name = "database.txt";
const int hash_capacity = 1000;

struct Node
{
    int key;
    char *val;
    struct Node *next;
};

void printList(struct Node* root) {
    struct Node *curr = root;
    while (curr != NULL)
    {
        printf("%d:%s\n", curr->key, curr->val);
        curr = curr->next;
    }
}

void appendOrSet(struct Node** root, int new_key, char *new_val)
{
    if (root == NULL) {
        printf("appendOrSet: pointer to root should not be null\n");
        return;
    }

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    struct Node *curr = *root;
    new_node->key = new_key;
    new_node->val = new_val;
    new_node->next = NULL;

    if (*root == NULL)
    {
        *root = new_node;
        return;
    }

    while (curr->next != NULL)
    {
        if (curr->key == new_key)
        {
            free(new_node);
            curr->val = new_val;
            return;
        }
        curr = curr->next;
    }
    if (curr->key == new_key)
    {
        free(new_node);
        curr->val = new_val;
        return;
    }

    curr->next = new_node;
    return;
}

char *get(struct Node* root, int key)
{
    struct Node *curr = root;
    while (curr != NULL)
    {
        if (curr->key == key)
        {
            return curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}

int deleteNode(struct Node** root, int key)
{
    if (root == NULL) {
        printf("deleteNode: pointer to root should not be null\n");
        return 0;
    }

    struct Node *curr = *root;
    struct Node *prev = *root;
 
    if (curr != NULL && curr->key == key) {
        *root = curr->next;
        free(curr);
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
    free(curr);
    return 1;
}

void freeAll(struct Node* node)
{
    if (node == NULL)
    {
        return;
    }
    freeAll(node->next);
    free(node);
}

int hash(int key)
{
    return key % hash_capacity;
}

int main(int argc, char *argv[])
{
    // // TODO hash table

    // // read db
    // FILE *file = fopen(db_name, "r");
    // if (file != NULL) {
    //     char *line = NULL;
    //     size_t linecap = 0;
    //     ssize_t linelen;
    //     while((linelen = getline(&line, &linecap, file)) > 0) {
    //         printf("input: %s", line);
    //     }

    //     // TODO add to hash table

    //     fclose(file);
    // }

    // for(int i = 1; i < argc; i++) {
    //     printf("%s\n", argv[i]);

    //     char *token, *string, *tofree;

    //     tofree = string = strdup(argv[i]);
    //     assert(string != NULL);

    //     while((token = strsep(&string, ",")) != NULL) {
    //         printf("  token:%s\n", token);

    //         // TODO check validity
    //     }

    //     free(tofree);

    //     // TODO if valid do something
    // }

    // // TODO store db
    // FILE *output_file = fopen(db_name, "w");
    // assert(output_file != NULL);

    // struct Node *root = NULL;
    // appendOrSet(&root, 1, "hello");
    // appendOrSet(&root, 2, "none");
    // appendOrSet(&root, 3, "python");
    // appendOrSet(&root, 2, "yippey");
    // appendOrSet(&root, 3, "go");
    // appendOrSet(&root, 4, "nnn");
    // int found = deleteNode(&root, 2);
    // assert(found);
    // printList(root);

    // int keys[] = {1, 3, 2, 4, 1004, 1002};
    // char *vals[] = {"hello", "none", "python", "go", "java", "c"};
    int keys[] = {4, 1004};
    char *vals[] = {"hello", "none"};
    struct Node *hash_table[hash_capacity];
    for(int i = 0; i < hash_capacity; i++) {
        hash_table[i] = NULL;
    }
    for(int i = 0; i < (sizeof keys / sizeof *keys); i++) {
        int hash_code = hash(keys[i]);
        appendOrSet(&hash_table[hash_code], keys[i], vals[i]);
    }

    printList(hash_table[4]);
    
    for(int i = 0; i < hash_capacity; i++) {
        freeAll(hash_table[i]);
    }
}