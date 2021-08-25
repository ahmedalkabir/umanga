//
// Created by Ahmed Alkabir on 9/27/2017.
//

#include <stdlib.h>
#include <string.h>

#include <ulist.h>

void list_init(uList *list, void (*destroy)(void *data)){

    list->size  = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;
}

void list_destroy(uList *list){
    void *data;

    while(list_size(list) > 0){
        if(list_rem_next(list,NULL,(void **)&data) == 0 && list->destroy != NULL){
            list->destroy(data);
        }
    }

    memset(list, 0, sizeof(uList));

    return;
}

int list_ins_next(uList *list, uListElement *element, const void *data){

    uListElement *new_element;

    if((new_element = (uListElement *)malloc(sizeof(uListElement))) == NULL)
        return -1;

    new_element->data = (void *)data;

    if(element == NULL){
        /** Handle insertion at the head of the list
         * */
        if(list_size(list) == 0)
            list->tail = new_element;

        new_element->next = list->head;
        list->head =  new_element;
    }else{
        /** Handle insertion somewhere other than at the head
         * */
        if(element->next == NULL)
            list->tail = new_element;
        new_element->next = element->next;
        element->next = new_element;
    }


    /// increase size of list
    list->size++;

    return 0;
}

int list_rem_next(uList *list, uListElement *element , void **data){
    uListElement *old_element;

    if(list_size(list) == 0)
        return -1;

    if(element == NULL){
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if(list_size(list) == 1)
            list->tail = NULL;
    }else{
        /** handle removal from somewhere other than the head
         * */
        if(element->next == NULL)
            return -1;
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if(element->next == NULL)
            list->tail = element;
    }

    /** Free the storage allocated by the abstract data type.
     * */
    free(old_element);

    /**
     *  decrease the size of the list
     * */
    list->size--;

    return 0;
}
