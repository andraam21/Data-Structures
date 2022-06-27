#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"
#include "Graph.h"

TGraphL *createGraphAdjList(int numberOfNodes)
{
	TGraphL *g = (TGraphL *)malloc(sizeof(TGraphL));
	g->nn = numberOfNodes;
	g->adl = (TNode **)malloc(g->nn * sizeof(TNode *));

	for (int i = 0; i < g->nn; i++)
	{
		g->adl[i] = NULL;
	}
	return g;
}

void addEdgeList(TGraphL *graph, int v1, int v2)
{
	TNode *n1 = (TNode *)malloc(sizeof(TNode));
	TNode *n2 = (TNode *)malloc(sizeof(TNode));

	n1->v = v2;
	n1->next = graph->adl[v1];
	graph->adl[v1] = n1;

	n2->v = v1;
	n2->next = graph->adl[v2];
	graph->adl[v2] = n2;
}

void dfsRecHelper(TGraphL *graph, int *visited, List *path, int s)
{
	visited[s] = 1;
	enqueue(path, s);

	for (TNode *it = graph->adl[s]; it; it = it->next)
	{
		if (!visited[it->v])
		{
			dfsRecHelper(graph, visited, path, it->v);
		}
	}
}

List *dfsRecursive(TGraphL *graph, int s)
{
	List *path = createList();
	int *visited = (int *)malloc(graph->nn * sizeof(int));

	for (int i = 0; i < graph->nn; i++)
	{
		visited[i] = 0;
	}

	dfsRecHelper(graph, visited, path, s);
	free(visited);
	return path;
}

List *bfs(TGraphL *graph, int s)
{
	List *path = createList();
	int *visited = (int *)malloc(graph->nn * sizeof(int));

	for (int i = 0; i < graph->nn; i++)
	{
		visited[i] = 0;
	}
	Queue *q = createQueue();

	enqueue(q, s);
	visited[s] = 1;

	while (!isQueueEmpty(q))
	{
		s = front(q);
		dequeue(q);

		enqueue(path, s);

		for (TNode *it = graph->adl[s]; it; it = it->next)
		{
			if (!visited[it->v])
			{
				visited[it->v] = 1;
				enqueue(q, it->v);
			}
		}
	}

	free(visited);
	destroyQueue(q);
	return path;
}

void destroyGraphAdjList(TGraphL *graph)
{
	for (int i = 0; i < graph->nn; i++)
	{
		TNode *it = graph->adl[i];

		while (it)
		{
			TNode *aux = it;
			it = it->next;
			free(aux);
		}
	}

	free(graph->adl);
	free(graph);
}

void removeEdgeList(TGraphL *graph, int v1, int v2)
{
	TNode *it = graph->adl[v1];
	TNode *prev = NULL;
	while (it != NULL && it->v != v2)
	{
		prev = it;
		it = it->next;
	}

	if (it == NULL)
		return;

	if (prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while (it != NULL && it->v != v1)
	{
		prev = it;
		it = it->next;
	}
	if (it == NULL)
		return;
	if (prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL *graph, int v)
{
	for (int i = 0; i < graph->nn; i++)
	{
		removeEdgeList(graph, v, i);
	}
}
