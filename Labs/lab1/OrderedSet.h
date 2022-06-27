#ifndef ORDERED_SET_H
#define ORDERED_SET_H

#include <stdio.h>  // needed for printf
#include <stdlib.h> // needed for alloc, realloc, free

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

/*
  OrderedSet represents a set with elements of type T stored in memory
  in an ordered fashion.
*/

typedef struct OrderedSet
{
    T *elements;   // pointer to where the elements are in memory
    long size;     // the number of elements in the set
    long capacity; // the current capacity (allocated memory)
} OrderedSet;

// -----------------------------------------------------------------------------

/*
  Function that creates an OrderedSet with elements of type T with a given
  initial capacity.
*/

OrderedSet *createOrderedSet(long initialCapacity)
{

    OrderedSet *newSet = (OrderedSet *)malloc(sizeof(OrderedSet));

    newSet->size = 0; // initially, the set is empty
    newSet->capacity = initialCapacity;
    newSet->elements = (T *)malloc(initialCapacity * sizeof(T));

    return newSet;
}

// -----------------------------------------------------------------------------

/*
  Function that checks if an element exists in a given set. As the
  elements of the set are ordered in memory, this function uses binary
  search.

  Function returns 1 if the given element exists in the set, and 0
  otherwise.
*/

int cmpfunc(const void *a, const void *b)
{
    return (*(T *)a - *(T *)b);
}

int contains(OrderedSet *set, const T element)
{
    qsort(set->elements, set->size, sizeof(T), cmpfunc);

    if (set->elements[set->size / 2] < element)
    {
        for (int i = set->size / 2; i < set->size; i++)
        {
            if (set->elements[i] == element)
            {
                return 1;
            }
        }
        return 0;
    }

    if (set->elements[set->size / 2] > element)
    {
        for (int i = 0; i < set->size / 2; i++)
        {
            if (set->elements[i] == element)
            {
                return 1;
            }
        }
        return 0;
    }

    // for (int i = 0; i < set->size; i++)
    // {
    //     if (set->elements[i] == element)
    //     {
    //         return 1;
    //     }
    // }
    // return 0;
}

// -----------------------------------------------------------------------------

/*
  Function that adds a new element to a set. If the element was
  already in the set, nothing changes. Otherwise, the element is added
  to the set. In the end, the elements must be in ascending order.
*/

void add(OrderedSet *set, const T newElement)
{
    if (set->size == set->capacity)
    {
        set->capacity = set->capacity * 2;
        set->elements = realloc(set->elements, set->capacity * sizeof(T));
    }

    // if (contains(set, newElement) == 0)
    // {
    //     for (int i = 0; i < set->size; i++)
    //     {
    //         if (set->elements[i] < newElement && newElement < set->elements[i + 1])
    //         {
    //             for (int j = set->size; j >= i + 2; j--)
    //             {
    //                 set->elements[j] = set->elements[j - 1];
    //             }
    //             set->elements[i + 1] = newElement;
    //         }
    //     }
    //     set->size++;
    // }

    if (contains(set, newElement) == 0)
    {
        set->elements[set->size] = newElement;
        set->size++;
    }
    qsort(set->elements, set->size, sizeof(T), cmpfunc);
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the union of
  the two
*/

OrderedSet *unionOrderedSets(OrderedSet *s1, OrderedSet *s2)
{
    int i = 0, j = 0, k = 0;
    int false = 0;
    int lung = s1->size + s2->size;
    if (s2->size > s1->size)
    {
        lung = s2->size;
    }
    OrderedSet *reunion = createOrderedSet(lung);

    for (i = 0; i < s1->size; i++)
    {
        reunion->elements[k] = s1->elements[i];
        k++;
    }

    for (j = 0; j < s2->size; j++)
    {
        for (i = 0; i < s1->size; i++)
        {
            if (s2->elements[j] == reunion->elements[i])
            {
                false = 1;
            }
        }
        if (false == 0)
        {
            reunion->elements[k] = s2->elements[j];
            k++;
        }
        false = 0;
    }

    reunion->size = k;
    qsort(reunion->elements, reunion->size, sizeof(T), cmpfunc);
    return reunion;

    //return createOrderedSet(1);
}

// -----------------------------------------------------------------------------

/*
  This function takes two ordered sets with elements of type T and it
  returns the addres of a new ordered set representing the
  intersection of the two
*/

OrderedSet *intersectOrderedSets(OrderedSet *s1, OrderedSet *s2)
{
    int i = 0, j = 0, k = 0;
    int lung = s1->size;
    if (s2->size > s1->size)
    {
        lung = s2->size;
    }
    OrderedSet *intersection = createOrderedSet(lung);

    for (i = 0; i < s1->size; i++)
    {
        for (j = 0; j < s2->size; j++)
        {
            if (s1->elements[i] == s2->elements[j])
            {
                intersection->elements[k] = s1->elements[i];
                k++;
            }
        }
    }
    intersection->size = k;
    qsort(intersection->elements, intersection->size, sizeof(T), cmpfunc);

    return intersection;
    //return createOrderedSet(1);
}

// -----------------------------------------------------------------------------

#endif
