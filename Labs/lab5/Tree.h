#ifndef TREE_H_D
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef int Item;

typedef struct Link
{
  Item elem;
  struct Link *l;
  struct Link *r;
} TreeNode;

void Init(TreeNode **t, Item x)
{
  TreeNode *tree = malloc(sizeof(TreeNode));
  tree->elem = x;
  tree->l = NULL;
  tree->r = NULL;
  *t = tree;
}

void Insert(TreeNode **t, Item x)
{
  TreeNode *tree = *t;
  if ((tree) == NULL)
  {
    Init(&tree, x);
  }
  if (x < tree->elem)
  {
    Insert(&(tree->l), x);
  }
  if (x > tree->elem)
  {
    Insert(&(tree->r), x);
  }
  *t = tree;
}

void PrintPostorder(TreeNode *t)
{
  if (t == NULL)
  {
    return;
  }
  PrintPostorder(t->l);
  PrintPostorder(t->r);
  printf("%d ", t->elem);
}

void PrintPreorder(TreeNode *t)
{
  if (t == NULL)
  {
    return;
  }
  printf("%d ", t->elem);
  PrintPreorder(t->l);
  PrintPreorder(t->r);
}

void PrintInorder(TreeNode *t)
{
  if (t == NULL)
  {
    return;
  }
  PrintInorder(t->l);
  printf("%d ", t->elem);
  PrintInorder(t->r);
}

void Free(TreeNode **t)
{
  while (*t != NULL)
  {
    Free(&(*t)->l);
    Free(&(*t)->r);
    free(*t);
  }
}

int Size(TreeNode *t)
{
}

int maxDepth(TreeNode *t)
{

  if (t == NULL)
  {
    return 0;
  }
  else
  {
    int hstanga = maxDepth(t->l);
    int hdreapta = maxDepth(t->r);
    int max = 0;
    if (hstanga > hdreapta)
    {
      max = hstanga;
    }
    else
    {
      max = hdreapta;
    }
    max++;
    return max;
  }
}

#endif // LINKEDSTACK_H_INCLUDED
