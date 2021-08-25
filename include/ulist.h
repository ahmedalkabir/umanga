//
// Created by Ahmed Alkabir on 9/27/2017.
//

#ifndef UMANGA_LIST_H
#define UMANGA_LIST_H

#include <stdlib.h>

/** Structure of Linked List Elements
 * */

typedef struct uListElmt_{
    void *data;
    struct uListElmt_ *next;
} uListElement;

/**
 *  Structure of linked lists
 * */
typedef struct uList_{
    int size;

    int (*match)(const void *key_1, const void *key_2);
    void (*destroy)(void *data);

    // head of linked list
    uListElement *head;

    // tail of linked list
    uListElement *tail;
}uList;

/** Interface of Linked lList
 * */
void list_init(uList *list, void(*destory)(void *data));

void list_destroy(uList *list);

int list_ins_next(uList *list, uListElement *element, const void *data);

int list_rem_next(uList *list, uListElement *element, void **data);

#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define list_is_tail(element)   ((element)->next == NULL ? 1 : 0)

#define list_data(element)  ((element)->data)

#define list_next(element)  ((element)->next)

#endif //UMANGA_LIST_H
