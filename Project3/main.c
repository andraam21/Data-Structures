#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

int dfs(TGraphL *g, int index, int *visited)
{
    visited[index] = 1;
    TNode *aux = g->adl[index];
    while (aux != NULL)
    {
        if (visited[aux->v] == 1)
        {
            return 1;
        }
        if (visited[aux->v] == 0)
        {
            // vizitam fiecare vecin nevizitat
            if (dfs(g, aux->v, visited) == 1)
            {
                return 1;
            }
        }
        aux = aux->next;
    }
    visited[index] = -1; // daca e "tips" il marcam cu -1
    // pentru ca poate fi un ciclu neorientat
    return 0;
}

int *past(TGraphL *g, int index, int *visited)
{
    TNode *aux = g->adl[index];
    while (aux != NULL)
    {
        // marcam cu 1 termenii vizitati, dupa care apelam recursiv functia
        visited[aux->v] = 1;
        visited = past(g, aux->v, visited);
        aux = aux->next;
    }
    return visited;
}

int *future(TGraphL *g, int index, int *visited)
{
    TNode *aux;
    for (int i = 0; i < g->nn; i++)
    {
        aux = g->adl[i];
        while (aux != NULL)
        {
            if (aux->v == index)
            {
                // marcam cu 1 termenii vizitati, dupa care apelam recursiv functia
                visited = future(g, i, visited);
                visited[i] = 1;
            }
            aux = aux->next;
        }
    }
    return visited;
}

int *tips(TGraphL *g, int index, int *visited)
{
    TNode *aux;
    for (int i = 0; i < g->nn; i++)
    {
        aux = g->adl[i];
        while (aux != NULL)
        {
            // daca nu e de tip "tips" il marcam ca vizitat
            visited[aux->v] = 1;
            aux = aux->next;
        }
    }
    return visited;
}

int main(int argc, char *argv[])
{
    // citim datele din fisier si cream graful
    int n;
    FILE *fin = fopen("blockdag.in", "r");
    fscanf(fin, "%d\n", &n);
    TGraphL *Graph = createGraphAdjList(n);
    char numeaux[256];
    char comanda[256];

    for (int i = 0; i < Graph->nn; i++)
    {
        fscanf(fin, "%s", numeaux);
        int lun = strlen(numeaux);
        Graph->nume[i] = malloc(lun * sizeof(char));
        strncpy(Graph->nume[i], numeaux, strlen(numeaux));
    }

    fscanf(fin, "\n");

    for (int i = 0; i < Graph->nn; i++)
    {
        fgets(comanda, sizeof(comanda), fin);
        comanda[strlen(comanda) - 1] = '\0';
        char *token = strtok(comanda, ":");
        token[strlen(token) - 1] = '\0';
        int good; // daca elementele sunt in alta ordine
        for (int k = 0; k < Graph->nn; k++)
        {
            if (strcmp(token, Graph->nume[k]) == 0)
            {
                good = k;
            }
        }
        token = strtok(NULL, ":");

        if (token != NULL)
        {
            token = strtok(token, " ");
            while (token != NULL)
            {
                TNode *newnode = malloc(sizeof(TNode));
                for (int j = 0; j < Graph->nn; j++)
                {
                    if (strcmp(token, Graph->nume[j]) == 0)
                    {
                        newnode->v = j;
                    }
                }
                newnode->next = Graph->adl[good];
                Graph->adl[good] = newnode;
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(fin);

    if (strcmp(argv[1], "-c1") == 0)
    {
        int c;
        int *visited = calloc(Graph->nn, sizeof(int));
        for (int i = 0; i < Graph->nn; i++)
        {
            c = dfs(Graph, i, visited);
            if (c == 1)
            {
                break;
            }
        }
        free(visited);

        FILE *fout = fopen("blockdag.out", "w");
        if (c == 0)
        {
            fprintf(fout, "correct\n");
        }
        else
        {
            fprintf(fout, "impossible\n");
        }
        fclose(fout);
    }

    if (strcmp(argv[1], "-c2") == 0)
    {
        // aflam indexul corespunzator numelui
        int index;
        for (int i = 0; i < Graph->nn; i++)
        {
            if (strcmp(argv[2], Graph->nume[i]) == 0)
            {
                index = i;
            }
        }

        int *ant = malloc(50 * sizeof(int));
        ant = past(Graph, index, ant);

        int *aft = malloc(50 * sizeof(int));
        aft = future(Graph, index, aft);

        int *tip = malloc(50 * sizeof(int));
        tip = tips(Graph, index, tip);

        FILE *fout = fopen("blockdag.out", "w"); // printam elementele in .out

        fprintf(fout, "past(%s) : ", Graph->nume[index]);
        for (int i = 0; i < Graph->nn; i++)
        {
            if (ant[i] == 1)
            {
                fprintf(fout, "%s ", Graph->nume[i]);
            }
        }
        fprintf(fout, "\n");

        fprintf(fout, "future(%s) : ", Graph->nume[index]);
        for (int i = 0; i < Graph->nn; i++)
        {
            if (aft[i] == 1)
            {
                fprintf(fout, "%s ", Graph->nume[i]);
            }
        }
        fprintf(fout, "\n");

        fprintf(fout, "anticone(%s) : ", Graph->nume[index]);
        for (int i = 0; i < Graph->nn; i++)
        {
            if (aft[i] != 1 && ant[i] != 1 && i != index)
            {
                fprintf(fout, "%s ", Graph->nume[i]);
            }
        }
        fprintf(fout, "\n");

        fprintf(fout, "tips(G) : ");
        for (int i = 0; i < Graph->nn; i++)
        {
            if (tip[i] != 1)
            {
                fprintf(fout, "%s ", Graph->nume[i]);
            }
        }
        fprintf(fout, "\n");

        fclose(fout);

        free(ant);
        free(aft);
        free(tip);
    }

    destroyGraphAdjList(Graph);
    return 0;
}
