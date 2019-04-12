///////////////////////////////////////////////////////////////////////////////
// llist.h
// Linked list implementation in C99. 
//
// Author: PotatoMaster101
// Date:   12/04/2019
///////////////////////////////////////////////////////////////////////////////

#ifndef LLIST_H
#define LLIST_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LLIST_OK 0
#define LLIST_NULL_ERR 1
#define LLIST_ALLOC_ERR 2

// The linked list node type. 
typedef struct linked_list_node_t {
    void *data;                             // internal data
    struct linked_list_node_t *prev;        // pointer to previous
    struct linked_list_node_t *next;        // pointer to next
} lnode_t;

// The linked list type. 
typedef struct linked_list_t {
    lnode_t *head;                          // list head
    lnode_t *tail;                          // list tail
    size_t len;                             // list size
} llist_t;

// Initialises the specified linked list. 
//
// PARAMS: 
// l - the linked list to initialise
//
// RET: 
// Zero on success, non-zero on error. 
int llist_init(llist_t *l);

// Returns the element at the given index in the specified linked list. If the 
// index is out of range, then the last element will be returned. 
//
// PARAMS: 
// l - the linked list to retrieve the element
// i - the index of the element
//
// RET: 
// The element at the given index, or NULL if any error occurred. 
void *llist_get(const llist_t *l, size_t i);

// Add a new element into the given linked list. The element will be stored as 
// a copy. 
//
// PARAMS: 
// l - the linked list to have the element added
// d - the element to add
// n - the size of the element
//
// RET: 
// Zero on success, non-zero on error. 
int llist_add(llist_t *l, const void *d, size_t n);

// Inserts a new element into the given linked list. The element will be 
// stored as a copy. 
//
// PARAMS: 
// l - the linked list to have the element inserted
// d - the element to insert
// n - the size of the element
// i - the index in the linked list to insert to
//
// RET: 
// Zero on success, non-zero on error. 
int llist_ins(llist_t *l, const void *d, size_t n, size_t i);

// Deletes the element at the given index in the specified linked list. If the 
// given index is out of range, then the last element will be deleted. 
//
// PARAMS: 
// l - the linked list to have the element deleted
// i - the index of the element
//
// RET: 
// The element that just got removed. 
void *llist_del(llist_t *l, size_t i);

// Clears the given linked list, removing and freeing every element. 
//
// PARAMS: 
// l - the linked list to free
void llist_clear(llist_t *l);

#endif

