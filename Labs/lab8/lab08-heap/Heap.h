#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct
{
	int prior; // Item priority
	Item data; // Item d
} ItemType;

typedef struct heap
{
	long maxHeapSize; // capacity
	long size;		  // number of elements
	ItemType *elem;	  // array of elements
} PriQueue, *APriQueue;

PriQueue *makeQueue(int maxHeapSize)
{
	PriQueue *h = (PriQueue *)malloc(sizeof(PriQueue));
	h->maxHeapSize = maxHeapSize;
	h->size = 0;
	h->elem = (ItemType *)malloc(sizeof(ItemType) * maxHeapSize);
	return h;
}

int getLeftChild(int i)
{
	int left;
	left = 2 * i + 1;
	return left;
}

int getRightChild(int i)
{
	int right;
	right = 2 * i + 2;
	return right;
}

int getParent(int i)
{
	int parent;
	parent = (i - 1) / 2;
	return parent;
}

ItemType getMax(APriQueue h)
{
	ItemType max;
	max = h->elem[0];
	return max;
}

void siftUp(APriQueue h, int idx)
{
	int p = getParent(idx);

	while (p >= 0 && h->elem[p].prior < h->elem[idx].prior)
	{
		ItemType aux = h->elem[p];
		h->elem[p] = h->elem[idx];
		h->elem[idx] = aux;
		idx = p;
		p = getParent(idx);
	}
}

void insert(PriQueue *h, ItemType x)
{
	if (h->size == h->maxHeapSize)
	{
		h->maxHeapSize = h->maxHeapSize * 2;
		h->elem = realloc(h->elem, sizeof(ItemType) * h->maxHeapSize);
	}
	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx)
{
	int left = getLeftChild(idx);
	int right = getRightChild(idx);

	if (left < h->size && right >= h->size)
	{
		if (h->elem[left].prior > h->elem[idx].prior)
		{
			ItemType aux = h->elem[left];
			h->elem[left] = h->elem[idx];
			h->elem[idx] = aux;
			idx = left;
			siftDown(h, idx);
		}
	}
	if (left < h->size && right < h->size)
	{
		int max;

		if (h->elem[left].prior > h->elem[right].prior)
		{
			max = left;
		}
		else
		{
			max = right;
		}

		if (h->elem[max].prior > h->elem[idx].prior)
		{
			ItemType aux = h->elem[max];
			h->elem[max] = h->elem[idx];
			h->elem[idx] = aux;
			idx = max;
			siftDown(h, idx);
		}
	}
}

void removeMax(APriQueue h)
{
	ItemType aux;
	aux = h->elem[0];
	h->elem[0] = h->elem[h->size - 1];
	h->elem[h->size - 1] = aux;
	h->size--;
	siftDown(h, 0);
}

void freeQueue(APriQueue h)
{
	free(h->elem);
	free(h);
}

#endif
