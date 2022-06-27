#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node
{
    T value;
    struct node *next;
} TNode, *TSortedList;

// TODO : Cerința 1 & Cerința 2

TSortedList create(T value)
{
    TSortedList lista = (TSortedList)malloc(sizeof(TSortedList));
    lista->value = value;
    lista->next = NULL;
    return lista;
}

int isEmpty(TNode *lista)
{
    if (lista == NULL)
    {
        return 1;
    }
    return 0;
}

int contains(TNode *lista, T element)
{
    TNode *aux = lista;
    while (aux != NULL)
    {
        if (aux->value == element)
        {
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

TNode *insert(TNode *lista, T element)
{
    TNode *n;

    if (isEmpty(lista))
    {
        lista = create(element);
        return lista;
    }

    if (element < lista->value)
    {
        n = (TNode *)malloc(sizeof(TNode));
        n->value = element;
        n->next = lista;
        lista = n;
        return lista;
    }

    n = lista;
    while (n->next != NULL && n->next->value < element)
    {
        n = n->next;
    }

    TNode *aux = n->next;
    n->next = malloc(sizeof(TNode));
    n->next->value = element;
    n->next->next = aux;

    return lista;
}

TNode *deleteOnce(TNode *lista, T element)
{
    TNode *aux = lista, *ant = NULL;

    while (aux != NULL && aux->value != element)
    {
        ant = aux;
        aux = aux->next;
    }

    if (isEmpty(lista))
    {
        return NULL;
    }

    ant->next = aux->next;
    free(aux);
    return lista;
}

long length(TNode *lista)
{
    long len = 0;
    TNode *aux = lista;
    while (aux != NULL)
    {
        len++;
        aux = aux->next;
    }
    return len;
}

T getNth(TNode *lista, int N)
{
    TNode *aux = lista;
    for (int i = 1; i < N; i++)
    {
        if (aux != NULL)
        {
            aux = aux->next;
        }
    }
    return aux->value;
}

TNode *freeTSortedList(TNode *lista)
{
    TNode *aux = lista;
    if (aux != NULL)
    {
        freeTSortedList(aux->next);
        free(aux);
    }
    return NULL;
}

#endif
