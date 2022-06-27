#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <string.h>

typedef struct HuffmanNode
{
    unsigned char value;
    struct HuffmanNode *left,
        *right,
        *parent;
} HuffmanNode, *PHuffmanNode;

typedef PHuffmanNode T;

#include "heap.h"

PHuffmanNode initNode(unsigned char value)
{
    HuffmanNode *node = malloc(sizeof(HuffmanNode));
    node->value = value;
    node->parent = NULL;
    node->right = NULL;
    node->left = NULL;
    return node;
}

void computeFreqs(char *text, int freqs[256])
{
    for (int j = 0; j <= 256; j++)
    {
        for (int i = 0; i < strlen(text); i++)
        {
            if (text[i] == j)
            {
                freqs[j]++;
            }
        }
    }
}

PHuffmanNode makeTree(int freqs[256])
{
    int capacity = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freqs[i] != 0)
        {
            capacity++;
        }
    }
    Heap *Heap = makeHeap(capacity);
    for (int i = 0; i < 256; i++)
    {
        if (freqs[i] != 0)
        {
            PHuffmanNode aux = initNode(i);
            insert(Heap, aux, freqs[i]);
        }
    }
    PHuffmanNode aux = malloc(sizeof(PHuffmanNode));
    while (Heap->size >= 2)
    {
        PHeapNode nod1 = getMin(Heap);
        removeMin(Heap);
        PHeapNode nod2 = getMin(Heap);
        removeMin(Heap);

        PHuffmanNode left = initNode(nod1->prior);
        PHuffmanNode right = initNode(nod2->prior);
        left->parent = aux;
        right->parent = aux;

        aux->value = nod2->prior + nod1->prior;
        aux->left = left;
        aux->right = right;
        aux->parent = NULL;
    }
    return aux;
}

void makeCodes(PHuffmanNode root, char **allCodes)
{
    int i, j = 0;
    for (i = 0; i < 256; i++)
    {
        while (root->value != i)
        {
            if (root->value < i)
            {
                makeCodes(root->left, allCodes);
                allCodes[i][j] = 0;
                j++;
            }
            else
            {
                makeCodes(root->right, allCodes);
                allCodes[i][j] = 1;
                j++;
            }
        }
        j = 0;
    }
}

char *compress(char *textToEncode, char **allCodes)
{
    return NULL;
}

char *decompress(char *textToDecode, PHuffmanNode root)
{
    return NULL;
}

#endif
