#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

void list_init(struct list_t *list)
{
    list->head = list->tail = NULL;
}

int list_add_node(struct list_t *list, const char *value,
                    const size_t size)
{
    struct node_t *ptr;
    ptr = (struct node_t *)malloc(sizeof(struct node_t));
    ptr->value = (char *)malloc(sizeof(char)*(size+1));

    if (ptr == NULL || ptr->value == NULL)
        return -1;

    if(list->head == NULL) {
        list->head = ptr;
    } else {
        list->tail->next = ptr;
    }
    list->tail = ptr;

    strncpy(ptr->value, value, size);
    ptr->value[size]='\0';
    ptr->next = NULL;
    return 0;
}

void list_trav(struct list_t *list, callback cb)
{
    struct node_t *ptr;
    if (list->head == NULL)
        return;

    for (ptr = list->head; ptr !=NULL; ptr = ptr->next) {
        cb(ptr->value);
    }
}

void list_free(struct list_t *list)
{
    struct node_t *ptr, *t;
    if (list->head == NULL) {
        return;
    }

    t = ptr = list->head;
    while (t) {
        t = ptr->next;
        free(ptr->value);
        free(ptr);
        ptr = t;
    }
}