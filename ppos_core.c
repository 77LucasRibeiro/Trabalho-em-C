//GRR20173546 LUCAS JOSÉ RIBEIRO

#include "ppos.h"
#include <stdio.h>
#include <stdlib.h>
#include "ppos_data.h"
#include <ucontext.h>

#define STACKSIZE 64*1024
    task_t principal, dis;
    task_t *a, *primeiro, *lista;

void dispatcher();
int scheduler();

void ppos_init (){
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
    setvbuf (stdout, 0, _IONBF, 0) ;
    getcontext(&principal.context);
    a = malloc(STACKSIZE);
    lista = malloc(STACKSIZE);
    primeiro = malloc(STACKSIZE);
    principal.context.uc_stack.ss_sp = malloc(STACKSIZE) ;
    principal.context.uc_stack.ss_size = STACKSIZE ;
    principal.context.uc_stack.ss_flags = 0 ;
    principal.context.uc_link = 0 ;
    principal.id = 0;
    principal.next = NULL;
    principal.prev = NULL;
    a=&principal;
    primeiro = &principal;
    lista = NULL;

    task_create(&dis, dispatcher, "dispatcher");

}

int task_create (task_t *task, void (*start_func)(void *), void *arg){
    getcontext(&task->context);
    int cont = a->id;
    task_t *aux;
    aux = lista;
    task->context.uc_stack.ss_sp = malloc(STACKSIZE) ;
    task->context.uc_stack.ss_size = STACKSIZE ;
    task->context.uc_stack.ss_flags = 0 ;
    task->context.uc_link = 0 ;
    if(a->next == NULL){
        task->id = cont+1;
        a->next = task;
        a->prev = task;
        task->next = a;
        task->prev = a;

    }else if(lista == NULL){
        task->id = a->next->id+1;
        lista = task;

        task->next = lista;
        task->prev = lista;
        }else{
            while(aux->next != lista){
                aux = aux->next;
            }
            cont = aux->id;
            task->id = cont+1;
            aux->next = task;
            task->prev = aux;
            task->next = lista;
            lista->prev = task;
        }
    if(start_func != NULL){
        makecontext (&task->context, (void*)(*start_func), 1, arg) ;
    }
    return(0);
}

void task_exit (int exitCode){
    if(a->id == dis.id){
        task_switch(&principal);
    }else{
        a->context.uc_stack.ss_flags = 1;
        task_switch(&dis);
    }
}

int task_switch (task_t *task){
    task_t *aux = a;
    a = task;
    swapcontext(&aux->context, &task->context);

    return(1);
}

int task_id (){
    return(a->id);
}

void task_yield (){
    task_switch(&dis);
}

void dispatcher(){
    task_t *aux = lista;
    aux = lista;
    while(lista != NULL){
        scheduler();
        aux = lista;
            task_switch(aux);
            if(aux->context.uc_stack.ss_flags == 1){
                if(aux->prev != aux){
                    aux->prev->next = aux->next;
                    aux->next->prev = aux->prev;
                    lista = aux->next;
                }else{
                    lista = NULL;
                }
            }
    }
    task_exit(0);
}

int scheduler(){
    if(lista != NULL){
        task_t *aux = lista->next;
        task_t *menor = lista;
        while(aux != lista){
            if(aux->Dpriority < menor->Dpriority){
                menor->Dpriority -=1;
                menor = aux;
            }else{
                aux->Dpriority -= 1;
                aux = aux->next;
            }
        }
        menor->Dpriority = menor->Epriority;
        lista = menor;
        return(1);
    }
    return(0);
}

void task_setprio (task_t *task, int prio){
    if(task != NULL){
        task->Epriority = prio;
        task->Dpriority = prio;
    }else{
        a->Epriority = prio;
        a->Dpriority = prio;
    }
}

int task_getprio (task_t *task){
    return(a->Epriority);
}
