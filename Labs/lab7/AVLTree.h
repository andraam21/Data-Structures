
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

// An AVL tree node
typedef struct AVLNode
{
	Item elem;
	int height;
	struct AVLNode *l;
	struct AVLNode *r;
	struct AVLNode *p;

} AVLNode;

typedef struct AVLTree
{
	long size;
	AVLNode *root;
	AVLNode *nil;
	int (*comp)(Item a, Item b);
} AVLTree;

AVLTree *avlCreateTree(int (*comp)(Item, Item))
{
	AVLTree *newTree = (AVLTree *)malloc(sizeof(AVLTree));
	newTree->comp = comp;
	newTree->nil = (AVLNode *)malloc(sizeof(AVLNode));
	newTree->nil->p = newTree->nil->l = newTree->nil->r = newTree->nil;
	newTree->nil->height = 0;
	newTree->root = (AVLNode *)malloc(sizeof(AVLNode));
	newTree->root->p = newTree->root->l = newTree->root->r = newTree->nil;
	newTree->root->height = 0;
	newTree->size = 0;

	return newTree;
}

int avlIsEmpty(AVLTree *tree)
{
	return (tree->root->l == tree->nil);
}

AVLNode *avlNewNode(AVLTree *tree)
{
	AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
	// Initialize the new node to be used in the tree
	newNode->p = newNode->r = newNode->l = tree->nil;
	newNode->height = 1;

	return newNode;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
	return (a > b) ? a : b;
}

// A utility function to right rotate subtree rooted with y
void avlRightRotate(AVLTree *tree, AVLNode *y)
{
	AVLNode *node;
	node = y->l;
	y->l = node->r;
	node->r = y;
	int ylheight = 0, yrheight = 0, nodelheight = 0;
	AVLNode *aux;
	aux = y;
	while (aux != NULL)
	{
		aux = aux->l;
		ylheight++;
	}
	aux = y;
	while (aux != NULL)
	{
		aux = aux->r;
		yrheight++;
	}
	aux = node;
	while (aux != NULL)
	{
		aux = aux->l;
		nodelheight++;
	}
	y->height = max(yrheight, ylheight) + 1;
	node->height = max(nodelheight, y->height) + 1;
}

// A utility function to left rotate subtree rooted with x
void avlLeftRotate(AVLTree *tree, AVLNode *x)
{
	AVLNode *node;
	node = x->r;
	x->r = node->l;
	node->l = x;
	int xlheight = 0, xrheight = 0, noderheight = 0;
	AVLNode *aux;
	aux = x;
	while (aux != NULL)
	{
		aux = aux->l;
		xlheight++;
	}
	aux = x;
	while (aux != NULL)
	{
		aux = aux->r;
		xrheight++;
	}
	aux = node;
	while (aux != NULL)
	{
		aux = aux->r;
		noderheight++;
	}
	x->height = max(xrheight, xlheight) + 1;
	node->height = max(noderheight, x->height) + 1;
}

// Get Balance factor of node x
int avlGetBalance(AVLNode *x)
{
	if (x == NULL)
		return 0;
	return x->l->height - x->r->height;
}

AVLNode *avlMinimum(AVLTree *tree, AVLNode *x)
{
	while (x->l != tree->nil)
		x = x->l;
	return x;
}

AVLNode *avlMaximum(AVLTree *tree, AVLNode *x)
{
	while (x->r != tree->nil)
	{
		x = x->r;
	}
	return x;
}

void avlInsert(struct AVLTree *tree, Item elem)
{
	if (tree == NULL)
		return (avlCreateTree(elem));

	if (elem < tree->root->elem)
		avlInsert(tree->root->l, elem);
	if (elem > tree->root->elem)
		avlInsert(tree->root->r, elem);

	int lh = 0, rh = 0;
	AVLNode *aux;
	aux = tree->root;
	while (aux != NULL)
	{
		aux = aux->l;
		lh++;
	}
	aux = tree->root;
	while (aux != NULL)
	{
		aux = aux->r;
		rh++;
	}
	tree->root->height = max(lh, rh);
	int balance = avlGetBalance(tree->root);
	if (balance > 1 && elem < tree->root->l->elem)
		return avlRightRotate(tree, elem);

	if (balance < -1 && elem > tree->root->r->elem)
		return avlLeftRotate(tree, elem);

	if (balance > 1 && elem > tree->root->l->elem)
	{
		avlLeftRotate(tree, tree->root->l);
		return avlRightRotate(tree, elem);
	}

	if (balance < -1 && elem < tree->root->r->elem)
	{
		avlRightRotate(tree, tree->root->r);
		return avlLeftRotate(tree, elem);
	}
}

void avlDeleteNode(AVLTree *tree, AVLNode *node)
{
	destroyElem(node->elem);
	free(node);
}

void avlDestroyTreeHelper(AVLTree *tree, AVLNode *x)
{
	if (x != tree->nil)
	{
		avlDestroyTreeHelper(tree, x->l);
		avlDestroyTreeHelper(tree, x->r);
		avlDeleteNode(tree, x);
	}
}

void avlDestroyTree(AVLTree *tree)
{
	avlDestroyTreeHelper(tree, tree->root->l);
	free(tree->root);
	free(tree->nil);
	free(tree);
}

#endif /* AVLTREE_H_ */