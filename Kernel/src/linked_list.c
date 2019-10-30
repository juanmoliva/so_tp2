
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <string.h>
#include <linked_list.h>

node_t* head;

/* Function to add a node. 
   This function expects a pointer to the data to be added 
   and size of the data type */
int add(node_t** head,void *new_data, size_t data_size) 
{ 
    // head should not be null
    if (head == NULL ){
        return -2;
    }

    // Allocate memory for node 
    node_t* new_node = (node_t*)malloc(sizeof(node_t)); 
    if (new_node == -1) {
        return -1;
    }

    new_node->data  = malloc(data_size); 
    if (new_node->data == -1) {
        return -1;
    }
    new_node->next = NULL; 

    node_t* current = (*head);
    while( current != NULL ) {
        current = current->next;
    }

    current = new_node;

    // Copy contents of new_data to newly allocated memory. 
    // Assumption: char takes 1 byte. 
    int i; 
    for (i=0; i<data_size; i++) {
        *(char *)(new_node->data + i) = *(char *)(new_data + i); 
    }

}

void *data( node_t* node ) {
    if ( node == NULL) {
        return NULL;
    }

    return node->data;
}

node_t *next(node_t* head, node_t* node ) {
    // node should not be NULL.
    if (node == NULL ){
        return NULL;
    }
    
    if (node->next == NULL ){
        return head;
    }
    else return node->next;
}