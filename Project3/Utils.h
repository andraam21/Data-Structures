typedef struct TNode
{
    int v;
    struct TNode *next;
} TNode;

typedef struct
{
    char **nume;
    int nn;
    TNode **adl;
} TGraphL;

TGraphL *createGraphAdjList(int numberOfNodes)
{
    TGraphL *g = (TGraphL *)malloc(sizeof(TGraphL));

    g->nn = numberOfNodes;
    g->adl = (TNode **)malloc(g->nn * sizeof(TNode *));
    g->nume = (char **)malloc(g->nn * sizeof(char *));

    for (int i = 0; i < g->nn; i++)
    {
        g->adl[i] = NULL;
    }

    return g;
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

        free(graph->nume[i]);
    }

    free(graph->nume);
    free(graph->adl);
    free(graph);
}