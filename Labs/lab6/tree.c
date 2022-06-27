/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Tree parent, Item value)
{
	Tree root = malloc(sizeof(TreeNode));
	root->parent = parent;
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
void init(Tree *root, Tree parent, Item value)
{
	*root = malloc(sizeof(TreeNode));
	(*root)->parent = parent;
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
	if (root == NULL)
	{
		init(&root, NULL, value);
		return root;
	}
	if (root->value == value)
	{
		return root;
	}
	if (root->value < value)
	{
		if (root->right == NULL)
		{
			init(&root->right, root, value);
		}
		else
		{
			insert(root->right, value);
		}
	}
	else
	{
		if (root->left == NULL)
		{
			init(&root->left, root, value);
		}
		else
		{
			insert(root->left, value);
		}
	}
	return root;
}

/*
*	Funcție care verifică dacă un arbore este vid sau nu
*		- întoarce 1 dacă arborele este vid
*		- întoarce 0 dacă arborele nu este vid
*/
int isEmpty(Tree root)
{
	if (root)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
*	Funcție care verifică dacă o valoare există sau nu în arbore
*		- 1 dacă value există în arbore
*		- 0 dacă value nu există în arbore
*/
int contains(Tree tree, Item value)
{
	if (tree == NULL)
	{
		return 0;
	}
	if (tree->value == value)
	{
		return 1;
	}
	if (value < tree->value)
	{
		tree = tree->left;
		contains(tree, value);
	}
	else
	{
		tree = tree->right;
		contains(tree, value);
	}
}

/*
*	Funcție care determină elementul minim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree minimum(Tree tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	if (tree->left == NULL)
	{
		return tree;
	}
	else
	{
		tree = tree->left;
		minimum(tree);
	}
}

/*
*	Funcție care determină elementul maxim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree maximum(Tree tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	if (tree->right == NULL)
	{
		return tree;
	}
	else
	{
		tree = tree->right;
		maximum(tree);
	}
}

/*
*	Funcție care determină succesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree successor(Tree root, Item value)
{
	if (root == NULL)
	{
		return NULL;
	}
	if (root->value == value)
	{
		if (root->right != NULL)
		{
			Tree tmp = root->right;
			while (tmp->left != NULL)
			{
				tmp = tmp->left;
			}
			return tmp;
		}
		else
		{
			Tree tmp = root->parent;
			while (tmp != NULL && root == tmp->right)
			{
				root = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
	}
	if (value < root->value)
	{
		Tree aux = root;
		aux = aux->left;
		successor(aux, value);
	}
	else
	{
		Tree aux = root;
		aux = aux->right;
		successor(aux, value);
	}
}

/*
*	Funcție care determină predecesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree predecessor(Tree root, Item value)
{
	if (root == NULL)
	{
		return NULL;
	}
	if (root->value == value)
	{
		if (root->left != NULL)
		{
			Tree tmp = root->left;
			while (tmp->right != NULL)
			{
				tmp = tmp->right;
			}
			return tmp;
		}
		else
		{
			Tree tmp = root->parent;
			while (tmp != NULL && root == tmp->left)
			{
				root = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
	}
	if (value < root->value)
	{
		Tree aux = root;
		aux = aux->left;
		predecessor(aux, value);
	}
	else
	{
		Tree aux = root;
		aux = aux->right;
		predecessor(aux, value);
	}
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void destroyTree(Tree *root)
{
	if (*root == NULL)
	{
		return;
	}
	else
	{
		destroyTree(&((*root)->left));
		destroyTree(&((*root)->right));
		free(*root);
		*root = NULL;
	}
}

/*
*	Funcție care șterge un nod cu o anumită valoare din arbore
*/
Tree delete (Tree root, Item value)
{

	if (root == NULL)
	{
		return NULL;
	}
	if (root->value > value)
	{
		root->left = delete (root->left, value);
	}
	else if (root->value < value)
	{
		root->right = delete (root->right, value);
	}
	else
	{
		if (root->left != NULL && root->right != NULL)
		{
			Tree tmp;
			tmp = minimum(root->right);
			root->value = tmp->value;
			root->right = delete (root->right, tmp->value);
		}
		else
		{
			Tree tmp;
			tmp = root;
			if (root->left)
			{
				root = root->left;
			}
			else
			{
				root = root->right;
			}
			free(tmp);
		}
	}
	return root;
}

/*
*	Funcție care determină cel mai apropiat strămoș comun pentru
* două noduri având cheile value1 și value2
*/
Tree lowestCommonAncestor(Tree root, Item value1, Item value2)
{
	if (root == NULL)
	{
		return NULL;
	}
	Tree aux;
	if (value1 < root->value && value2 < root->value)
	{
		aux = root;
		aux = aux->left;
		lowestCommonAncestor(aux, value1, value2);
	}
	if (value1 > root->value && value2 > root->value)
	{
		aux = root;
		aux = aux->right;
		lowestCommonAncestor(aux, value1, value2);
	}
	return aux;
}
