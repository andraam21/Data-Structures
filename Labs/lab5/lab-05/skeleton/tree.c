/*
*	Created by Nan Mihai on 28.03.2021
*	Copyright (c) 2021 Nan Mihai. All rights reserved.
*	Laborator 5 - Structuri de date
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Item value)
{
	Tree root = malloc(sizeof(TreeNode));
	root->value = value;
	root->left = NULL;
	root->right = NULL;
	return root;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Item value)
{
	if (root == NULL)
	{
		createTree(value);
	}
	(*root)->value = value;
	(*root)->left = NULL;
	(*root)->right = NULL;
}
/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value)
{

	Tree tree = root;
	if ((tree) == NULL)
	{
		init(&tree, value);
	}
	if (value < (tree)->value)
	{
		insert(((tree)->left), value);
	}
	if (value > (tree)->value)
	{
		insert(((tree)->right), value);
	}
	root = tree;

	return root;
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în postordine
*/
void printPostorder(Tree root)
{
	if (root == NULL)
	{
		return;
	}
	printPostorder(root->left);
	printPostorder(root->right);
	printf("%d ", root->value);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în preordine
*/
void printPreorder(Tree root)
{
	if (root == NULL)
	{
		return;
	}
	printf("%d ", root->value);
	printPreorder(root->left);
	printPreorder(root->right);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în inordine
*/
void printInorder(Tree root)
{
	if (root == NULL)
	{
		return;
	}
	printInorder(root->left);
	printf("%d ", root->value);
	printInorder(root->right);
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void freeTree(Tree *root)
{
	// TODO 6
}

/*
*	Funcție care determină numărul de noduri dintr-un arbore binar
*/
int size(Tree root)
{
	// TODO 7
	return -1;
}

/*
*	Funcție care returnează adâncimea maximă a arborelui
*/
int maxDepth(Tree root)
{
	// TODO 8
	return -1;
}

/*
*	Funcție care construiește oglinditul unui arbore binar
*/
void mirror(Tree root)
{
	Tree tree = root;
	if (tree == NULL)
	{
		return;
	}

	else
	{
		TreeNode *aux;
		mirror(tree->left);
		mirror(tree->right);

		aux = tree->left;
		tree->left = tree->right;
		tree->right = aux;
	}
	root = tree;
}

/*
*	Funcție care verifică dacă doi arbori binari sunt identici
*/
int sameTree(Tree root1, Tree root2)
{

	if (root1 == NULL && root2 == NULL)
	{
		return 1;
	}

	if (root1 != NULL && root2 != NULL)
	{
		return (
			root1->value == root2->value &&
			sameTree(root1->left, root2->left) &&
			sameTree(root1->right, root2->right));
	}

	return 0;
}
