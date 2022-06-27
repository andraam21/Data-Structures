#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode
{
	Item elem;			   // Stored node value
	struct ListNode *next; // link to next node
	struct ListNode *prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List
{
	ListNode *first; // link to the first node
	ListNode *last;	 // link to the last node
} List;
// -----------------------------------------------------------------------------

/**
 * Create a new node
 *  [input]: Item
 *  [output]: List*
 */
ListNode *createNode(Item elem)
{
	ListNode *node = malloc(sizeof(ListNode));

	node->elem = elem;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List *createList(void)
{
	List *list = malloc(sizeof(List));

	list->first = NULL;
	list->last = NULL;

	return list;
}
// -----------------------------------------------------------------------------

/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */

int isEmpty(List *list)
{
	return (list == NULL) || (list->first == NULL);
}
// -----------------------------------------------------------------------------

/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem)
{
	if (isEmpty(list))
		return 0;

	ListNode *ptr = list->first;

	while (ptr)
	{
		if (ptr->elem == elem)
			return 1;

		ptr = ptr->next;
	}

	return 0;
}
// -----------------------------------------------------------------------------

/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List *list, Item elem, int pos)
{

	// Guard against young player errors
	if (!list)
		return;

	ListNode *ptr = list->first;

	while (pos && ptr)
	{
		ptr = ptr->next;
		pos--;
	}

	if (!pos)
	{
		ListNode *newNode = createNode(elem);

		// Corner case 1: empty list
		if (isEmpty(list))
		{
			list->first = newNode;
			list->last = newNode;
			return;
		}

		// Corner case 2: inserting at the end of the list
		if (ptr == NULL)
		{
			newNode->prev = list->last;
			list->last->next = newNode;
			list->last = newNode;
			return;
		}

		// Corner case 3: inserting at the beginning of the list
		if (ptr->prev == NULL)
		{
			newNode->next = ptr;
			ptr->prev = newNode;
			list->first = newNode;
			return;
		}

		// Normal case
		newNode->next = ptr;
		newNode->prev = ptr->prev;
		ptr->prev->next = newNode;
		ptr->prev = newNode;
	}
}
// -----------------------------------------------------------------------------

/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem)
{
	// Guard against young player errors
	if (!list)
		return;

	if (list->first == list->last)
	{
		free(list->first);
		list->first = NULL;
		list->last = NULL;
	}

	ListNode *ptr = list->first;

	while (ptr)
	{
		if (ptr->elem == elem)
		{
			// Removing first element
			if (ptr == list->first)
			{
				list->first = ptr->next;
				ptr->next->prev = NULL;
				free(ptr);
				return;
			}

			// Removing last element
			if (ptr == list->last)
			{
				list->last = ptr->prev;
				ptr->prev->next = NULL;
				free(ptr);
				return;
			}
			// Normal case
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;
			free(ptr);
			return;
		}
		ptr = ptr->next;
	}
}
// -----------------------------------------------------------------------------

/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list)
{
	// Guard against young player errors
	if (!list)
		return 0;

	ListNode *ptr = list->first;
	int len = 0;

	while (ptr)
	{
		len++;
		ptr = ptr->next;
	}

	return len;
}
// -----------------------------------------------------------------------------

/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List *destroyList(List *list)
{
	// Guard against young player errors
	if (!list)
		return 0;

	if (list->first == list->last)
	{
		free(list->first);
		free(list);
		return list;
	}

	ListNode *ptr = list->first;

	while (ptr)
	{
		ListNode *tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}

	free(list);

	return list;
}
// -----------------------------------------------------------------------------

#endif //_DOUBLE_LINKED_LIST_H_
