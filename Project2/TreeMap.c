#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree *createTree(void *(*createElement)(void *),
				  void (*destroyElement)(void *),
				  void *(*createInfo)(void *),
				  void (*destroyInfo)(void *),
				  int compare(void *, void *))
{
	TTree *tree = (TTree *)malloc(sizeof(TTree));
	tree->root = NULL;
	tree->size = 0;
	tree->compare = compare;
	tree->createElement = createElement;
	tree->createInfo = createInfo;
	tree->destroyElement = destroyElement;
	tree->destroyInfo = destroyInfo;
	return tree;
}

/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree *tree)
{
	if (tree == NULL || tree->root == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode *search(TTree *tree, TreeNode *x, void *elem)
{
	if (x == NULL)
	{
		return NULL;
	}
	if (tree->compare(x->elem, elem) == 0)
	{
		return x;
	}
	if (tree->compare(x->elem, elem) == 1)
	{
		return search(tree, x->left, elem);
	}
	if (tree->compare(x->elem, elem) == -1)
	{
		return search(tree, x->right, elem);
	}
}

/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode *minimum(TreeNode *x)
{
	if (x == NULL)
	{
		return NULL;
	}
	if (x->left == NULL)
	{
		return x;
	}
	else
	{
		return minimum(x->left);
	}
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode *maximum(TreeNode *x)
{
	if (x == NULL)
	{
		return NULL;
	}
	if (x->right == NULL)
	{
		return x;
	}
	else
	{
		return maximum(x->right);
	}
}

/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode *successor(TreeNode *x)
{
	TreeNode *p, *aux;
	if (x == NULL)
	{
		return NULL;
	}
	if (x->right != NULL)
	{
		// cel mai mic element din arborele drept
		p = x->right;
		return minimum(p);
	}
	if (x->right == NULL)
	{
		p = x->parent;
		aux = x;
		// cat timp venim din dreapta arborelui urcam in acesta
		while (p != NULL && aux == p->right)
		{
			aux = p;
			p = p->parent;
		}
		return p;
	}
}

/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode *predecessor(TreeNode *x)
{
	TreeNode *p, *aux;
	if (x == NULL)
	{
		return NULL;
	}
	if (x->left != NULL)
	{
		// cel mai mare element din arborele stang
		p = x->left;
		return maximum(p);
	}
	if (x->left == NULL)
	{
		p = x->parent;
		aux = x;
		// cat timp venim din stanga arborelui urcam in acesta
		while (p != NULL && aux == p->left)
		{
			aux = p;
			p = p->parent;
		}
		return p;
	}
}

/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode *x)
{

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL)
	{
		if (x->left != NULL)
		{
			leftHeight = x->left->height;
		}
		if (x->right != NULL)
		{
			rightHeight = x->right->height;
		}
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree *tree, TreeNode *x)
{
	if (isEmpty(tree) || x == NULL)
	{
		return;
	}
	if (x->right == NULL)
	{
		return;
	}
	TreeNode *y = x->right, *p = x->parent;
	if (tree->root == x)
	{
		// daca rotatia se face din radacina aceasta trebuie actualizata
		tree->root = y;
		y->parent = NULL;
	}
	else
	{
		// actualizam parintele
		if (p->right == x)
		{
			p->right = y;
		}
		else
		{
			p->left = y;
		}
	}
	x->right = y->left;
	if (y->left != NULL)
	{
		// daca exista elementul ii actualizam parintele
		y->left->parent = x;
	}
	// facem rotatia si actualizam inaltimile
	y->left = x;
	x->parent = y;
	y->parent = p;
	updateHeight(p);
	updateHeight(y);
	updateHeight(x);
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree *tree, TreeNode *y)
{
	if (isEmpty(tree) || y == NULL)
	{
		return;
	}
	if (y->left == NULL)
	{
		return;
	}
	TreeNode *x = y->left, *p = y->parent;
	if (tree->root == y)
	{
		// daca rotatia se face din radacina aceasta trebuie actualizata
		tree->root = x;
		x->parent = NULL;
	}
	else
	{
		// actualizam parintele
		if (p->right == y)
		{
			p->right = x;
		}
		else
		{
			p->left = x;
		}
	}
	y->left = x->right;
	if (x->right != NULL)
	{
		// daca exista elementul ii actualizam parintele
		x->right->parent = y;
	}
	// facem rotatia si actualizam inaltimile
	x->right = y;
	y->parent = x;
	x->parent = p;
	updateHeight(p);
	updateHeight(x);
	updateHeight(y);
}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x)
{
	int xl = 0, xr = 0;
	if (x == NULL)
	{
		return 0;
	}
	// daca nodurile nu sunt nule salvam inaltimile
	if (x->left != NULL)
	{
		xl = x->left->height;
	}
	if (x->right != NULL)
	{
		xr = x->right->height;
	}
	// aflam diferenta dintre arborele stang si cel drept
	return xl - xr;
}

/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree *tree, TreeNode *y)
{
	if (y == NULL)
	{
		return;
	}
	// balansam arborele incepand de la frunze la radacina
	while (y != NULL)
	{
		// arborele din dreapta este mai inalt
		if (avlGetBalance(y) == -2)
		{
			// daca este de tip zig-zag facem doua rotatii
			if (avlGetBalance(y->right) == 1)
			{
				avlRotateRight(tree, y->right);
				avlRotateLeft(tree, y);
			}
			else
			{
				avlRotateLeft(tree, y);
			}
		}
		// arborele din stanga este mai inalt
		if (avlGetBalance(y) == 2)
		{
			// daca este de tip zig-zag facem doua rotatii
			if (avlGetBalance(y->left) == -1)
			{
				avlRotateLeft(tree, y->left);
				avlRotateRight(tree, y);
			}
			else
			{
				avlRotateRight(tree, y);
			}
		}
		updateHeight(y);
		updateHeight(y->parent);
		y = y->parent;
	}
}

/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode *createTreeNode(TTree *tree, void *value, void *info)
{

	if (tree == NULL)
	{
		return NULL;
	}

	// Alocarea memoriei
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;

	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}

/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree *tree, void *elem, void *info)
{
	if (tree == NULL)
	{
		return;
	}
	// daca arborele este gol inseram in radacina noul nod
	if (isEmpty(tree))
	{
		TreeNode *newnode = createTreeNode(tree, elem, info);
		newnode->end = newnode;
		tree->root = newnode;
		tree->size++;
		return;
	}
	TreeNode *newnode = createTreeNode(tree, elem, info);
	newnode->end = newnode; // pregatim nodul pentru lista
	TreeNode *x = tree->root;
	TreeNode *y = NULL;
	// gasim pozitia la care trebuie inserat noul nod
	while (x != NULL)
	{
		y = x;
		if (tree->compare(elem, x->elem) == -1)
		{
			x = x->left;
		}
		else if (tree->compare(elem, x->elem) == 1)
		{
			x = x->right;
		}
		else
		{
			// daca nodul exista nu il mai adaugam in arbore, ci doar in lista
			TreeNode *ins = search(tree, tree->root, elem);

			newnode->next = ins->end->next;
			if (ins->end->next != NULL)
			{
				ins->end->next->prev = newnode;
			}
			newnode->prev = ins->end;
			ins->end->next = newnode;
			ins->end = newnode; // actualizam finalul listei de dubluri

			return;
		}
	}
	// inseram nodul
	if (y == NULL)
	{
		y = newnode;
	}
	else if (tree->compare(elem, y->elem) == -1)
	{
		y->left = newnode;
		newnode->parent = y;
	}
	else
	{
		y->right = newnode;
		newnode->parent = y;
	}
	// inseram nodul si in lista
	TreeNode *pred = predecessor(newnode);
	TreeNode *suc = successor(newnode);
	if (suc != NULL)
	{
		newnode->next = suc;
		suc->prev = newnode;
	}
	if (pred != NULL)
	{
		newnode->prev = pred->end;
		pred->end->next = newnode;
	}
	avlFixUp(tree, y);
	tree->size++;
}

/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode *node)
{

	// Verificarea argumentelor functiei
	if (tree == NULL || node == NULL)
		return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}

/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete (TTree *tree, void *elem)
{
	if (isEmpty(tree))
	{
		return;
	}
	TreeNode *del = search(tree, tree->root, elem);
	if (del == NULL)
	{
		return;
	}
	// stergem elementul din lista
	if (del->end != del)
	{
		TreeNode *aux = del->end;
		if (aux->prev != NULL)
		{
			aux->prev->next = aux->next;
		}
		if (aux->next != NULL)
		{
			aux->next->prev = aux->prev;
		}
		del->end = aux->prev;
		destroyTreeNode(tree, aux);
		return;
	}
	if (del->next != NULL)
	{
		del->next->prev = del->prev;
	}
	if (del->prev != NULL)
	{
		del->prev->next = del->next;
	}
	TreeNode *par = del->parent, *ri = del->right, *le = del->left;
	if (ri == NULL)
	{
		if (le == NULL)
		{
			if (par != NULL)
			{
				// daca noul ce trebuie sters este de tip frunza
				if (par->left == del)
				{
					par->left = NULL;
				}
				if (par->right == del)
				{
					par->right = NULL;
				}
			}
			// daca e radacina o actualizam
			if (del == tree->root)
			{
				tree->root = NULL;
			}
			destroyTreeNode(tree, del);
			avlFixUp(tree, par);
		}
		else
		{
			if (par != NULL)
			{
				// daca nodul ce trebuie sters are un element in stanga
				if (par->left == del)
				{
					par->left = le;
				}
				if (par->right == del)
				{
					par->right = le;
				}
			}
			// daca e radacina o actualizam
			if (del == tree->root)
			{
				tree->root = le;
			}
			le->parent = par;
			destroyTreeNode(tree, del);
			avlFixUp(tree, par);
		}
	}
	if (le == NULL && ri != NULL)
	{
		if (par != NULL)
		{
			// daca nodul ce trebuie sters are un element in dreapta
			if (par->left == del)
			{
				par->left = ri;
			}
			if (par->right == del)
			{
				par->right = ri;
			}
		}
		// daca e radacina o actualizam
		if (del == tree->root)
		{
			tree->root = ri;
		}
		ri->parent = par;
		destroyTreeNode(tree, del);
		avlFixUp(tree, par);
	}
	if (le != NULL && ri != NULL)
	{
		// daca nodul ce trebuie sters are doi copii
		// ii gasim succesorul si il mutam pe locul nodului de sters
		// succesorul poate avea elemente doar in dreapta
		TreeNode *suc = successor(del);
		if (suc->parent != NULL)
		{
			if (suc->parent->left == suc)
			{
				suc->parent->left = suc->right;
			}
			if (suc->parent->right == suc)
			{
				suc->parent->right = suc->right;
			}
		}
		if (suc->right != NULL)
		{
			suc->right->parent = suc->parent;
		}
		// verificam daca e radacina pentru a o actualiza
		if (tree->root == del)
		{
			tree->root = suc;
		}
		if (par != NULL)
		{
			if (par->left == del)
			{
				par->left = suc;
			}
			if (par->right == del)
			{
				par->right = suc;
			}
		}
		suc->parent = del->parent;
		// atribuim copiii elementului ce trebuie sters succesorului
		suc->left = del->left;
		if (suc->left != NULL)
		{
			suc->left->parent = suc;
		}
		suc->right = del->right;
		if (suc->right != NULL)
		{
			suc->right->parent = suc;
		}
		destroyTreeNode(tree, del);
		avlFixUp(tree, par);
	}
	tree->size--;
	return;
}

/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree *tree)
{
	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL)
	{
		return;
	}
	TreeNode *aux = minimum(tree->root);
	while (aux->next != NULL)
	{
		aux = aux->next;
		destroyTreeNode(tree, aux->prev);
	}
	destroyTreeNode(tree, aux);
	free(tree);
}
