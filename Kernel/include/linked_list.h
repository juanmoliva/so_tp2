#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string.h>

typedef struct node {
    void *data;
    struct node * next;
} node_t;

int add(void ** head,void *new_data, size_t data_size);
void *data( node_t* node );
node_t *next(node_t* head, node_t* node );
#endif