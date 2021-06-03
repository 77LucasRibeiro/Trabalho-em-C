//GRR20173546 Lucas José Ribeiro
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int queue_size (queue_t *queue){
    int cont = 0;
    if (queue == NULL){
        return (cont);
    }else{
        cont = cont + 1;
        queue_t *aux = queue;
        while(aux->next != queue){
            cont = cont + 1;
            aux = aux->next;
        }
        return(cont);
    }
}

void queue_print (char *name, queue_t *queue, void print_elem (void*)){
    printf("%s: [", name);
    if(queue == NULL)
        printf("]\n");
    else{
        queue_t *aux = NULL;
        aux = queue;
        print_elem(aux);
        while(aux->next != queue){
            printf(" ");
            aux = aux->next;
            print_elem(aux);
        }
    printf("]\n");
    }
}

int queue_append (queue_t **queue, queue_t *elem){
    if(elem->next && elem->prev !=NULL)
        return(0);
    if (*queue == NULL){
        *queue = elem;
        elem->prev = elem;
        elem->next = elem;
    }else{
        queue_t *aux = *queue;
        while(aux->next != *queue){
            aux = aux->next;
        }
        aux->next = elem;
        elem->next = *queue;
        elem->prev = aux;
        (*queue)->prev = elem;
    }
    return(0);
}

int queue_remove (queue_t **queue, queue_t *elem){
    queue_t *aux = *queue;
    while(aux != elem && aux->next != *queue){
        aux = aux->next;
    }
    if(aux != elem)
        return(0);
    if(*queue == elem){
        if((*queue)->next && (*queue)->prev != *queue){
            (*queue)->next->prev = (*queue)->prev;
            (*queue)->prev->next = (*queue)->next;
            (*queue)->prev = NULL;
            queue_t *aux = *queue;
            *queue = (*queue)->next;
            aux->next = NULL;
        }else{
            queue_t *aux = *queue;
            aux->prev = NULL;
            aux->next = NULL;
            *queue = NULL;
        }
    }else{
        elem->next->prev = elem->prev;
        elem->prev->next = elem->next;
        elem->prev = NULL;
        elem->next = NULL;
    }
    return(0);
}
