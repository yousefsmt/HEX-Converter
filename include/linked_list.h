#ifndef INCLUDE_LINKED_LIST_H
#define INCLUDE_LINKED_LIST_H

#include <stdio.h>

struct node_t
{
    char *value;
    struct node_t *next;
};

struct list_t
{
    struct node_t *head;
    struct node_t *tail;
};

typedef void (*callback)(const char *);

void list_init(struct list_t *list);
int list_add_node(struct list_t *list,
    const char *value, const size_t size);
void list_free(struct list_t *list);
void list_trav(struct list_t *list, callback cb);

#endif /* INCLUDE_LINKED_LIST_H */