///////////////////////////////////////////////////////////////////////////////
// llist.c
// Linked list implementation in C99. 
//
// Author: PotatoMaster101
// Date:   12/04/2019
///////////////////////////////////////////////////////////////////////////////

#include "llist.h"

static inline _Bool llist_empty(const llist_t *l);
static lnode_t *lnode_new(const void *d, size_t n);
static lnode_t *lnode_get(const llist_t *l, size_t i);
static void lnode_free_whole(lnode_t *n);
static void *lnode_free(lnode_t *n);

// Initialises the specified linked list. 
//
// PARAMS: 
// l - the linked list to initialise
//
// RET: 
// Zero on success, non-zero on error. 
int llist_init(llist_t *l) {
    if (l == NULL)
        return LLIST_NULL_ERR;

    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
    return LLIST_OK;
}

// Returns the element at the given index in the specified linked list. If the 
// index is out of range, then the last element will be returned. 
//
// PARAMS: 
// l - the linked list to retrieve the element
// i - the index of the element
//
// RET: 
// The element at the given index, or NULL if any error occurred. 
void *llist_get(const llist_t *l, size_t i) {
    lnode_t *node = lnode_get(l, i);
    void *ret = NULL;
    if (node != NULL)
        ret = node->data;
    return ret;
}

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
int llist_add(llist_t *l, const void *d, size_t n) {
    if (l == NULL || d == NULL || n == 0)
        return LLIST_NULL_ERR;

    int ret = LLIST_ALLOC_ERR;
    lnode_t *add = lnode_new(d, n);
    if (add != NULL) {
        ret = LLIST_OK;
        if (l->len == 0) {      // list is empty
            l->head = add;
            l->tail = add;
        } else {
            l->tail->next = add;
            add->prev = l->tail;
            l->tail = l->tail->next;
        }
        l->len++;
    }
    return ret;
}

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
int llist_ins(llist_t *l, const void *d, size_t n, size_t i) {
    if (l == NULL || d == NULL || n == 0)
        return LLIST_NULL_ERR;
    if (l->len == 0 || i >= l->len)
        return llist_add(l, d, n);      // let llist_add handle out of range

    int ret = LLIST_ALLOC_ERR;
    lnode_t *ins = lnode_new(d, n);
    if (ins != NULL) {
        ret = LLIST_OK;
        if (i == 0) {       // insert to head
            l->head->prev = ins;
            ins->next = l->head;
            l->head = l->head->prev;
        } else {            // insert to mid
            lnode_t *bef = lnode_get(l, i - 1);
            lnode_t *aft = bef->next;
            bef->next = ins;
            ins->prev = bef;
            aft->prev = ins;
            ins->next = aft;
        }
        l->len++;
    }
    return ret;
}

// Deletes the element at the given index in the specified linked list. If the 
// given index is out of range, then the last element will be deleted. 
//
// PARAMS: 
// l - the linked list to have the element deleted
// i - the index of the element
//
// RET: 
// The element that just got removed. 
void *llist_del(llist_t *l, size_t i) {
    lnode_t *node = lnode_get(l, i);    // returns last if out of range
    if (node != NULL) {
        if (l->len == 1) {              // final element, clear list
            l->head = NULL;
            l->tail = NULL;
        } else if (i == 0) {            // first element, update head
            l->head = l->head->next;
            l->head->prev = NULL;
        } else if (i >= l->len - 1) {   // last element, update tail
            l->tail = l->tail->prev;
            l->tail->next = NULL;
        } else {                        // delete mid, update link
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        l->len--;
    }
    return lnode_free(node);
}

// Clears the given linked list, removing and freeing every element. 
//
// PARAMS: 
// l - the linked list to free
void llist_clear(llist_t *l) {
    if (!llist_empty(l)) {
        lnode_t *current = l->head;
        while (current != NULL) {
            lnode_t *n = current;
            current = current->next;
            lnode_free_whole(n);
        }
        l->len = 0;
        l->head = NULL;
        l->tail = NULL;
    }
}

// Returns whether the given linked list is empty or not. 
//
// PARAMS: 
// l - the linked list to check
//
// RET: 
// True (1) if the linked list is empty, 0 (false) otherwise. 
static inline _Bool llist_empty(const llist_t *l) {
    return (l == NULL || l->len == 0);
}

// Returns a dynamically allocated linked list node. 
//
// PARAMS: 
// d - the data in the node
// n - the size of data
//
// RET: 
// The new node allocated, or NULL if any error occurred. 
static lnode_t *lnode_new(const void *d, size_t n) {
    lnode_t *ret = malloc(sizeof *ret);
    if (ret != NULL) {
        ret->prev = NULL;
        ret->next = NULL;
        ret->data = malloc(n);
        if (ret->data == NULL) {
            free(ret);
            ret = NULL;
        } else {
            memcpy(ret->data, d, n);
        }
    }
    return ret;
}

// Returns the linked list node at the specified index in the linked list. 
// If the index is out of range, the last node will be returned. 
//
// PARAMS: 
// l - the linked list to retrieve the node
// i - the index of the node
//
// RET: 
// The node retrieved, or NULL if any error occurred. 
static lnode_t *lnode_get(const llist_t *l, size_t i) {
    if (llist_empty(l))
        return NULL;

    lnode_t *ret = NULL;
    i = (i >= l->len) ? (l->len - 1) : i;
    if (i >= l->len / 2) {      // node in upper half
        ret = l->tail;
        for (size_t j = 0; j < (l->len - i - 1); j++)
            ret = ret->prev;
    } else {                    // node in lower half
        ret = l->head;
        for (size_t j = 0; j < i; j++)
            ret = ret->next;
    }
    return ret;
}

// Frees the given linked list node and the element inside. 
//
// PARAMS: 
// n - the node to free
static void lnode_free_whole(lnode_t *n) {
    if (n != NULL) {
        free(n->data);
        n->prev = NULL;     // incase access after free
        n->next = NULL;
        n->data = NULL;
        free(n);
    }
}

// Frees the given linked list node and return the internal element. 
//
// PARAMS: 
// n - the node to free
static void *lnode_free(lnode_t *n) {
    void *ret = NULL;
    if (n != NULL) {
        ret = n->data;
        n->prev = NULL;     // incase access after free
        n->next = NULL;
        n->data = NULL;
        free(n);
    }
    return ret;
}

