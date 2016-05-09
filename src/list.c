/* ========================================================================== */
/* File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Victoria M. Taylor
 * Date: July 25, 2015
 *
 * Functions to append and pop WebPage structs from a doubly-linked List
 */
/* ========================================================================== */


#include <stdlib.h>
#include <stdio.h>
#include "list.h"


/*Inserts a WebPage into a new ListNode and appends it to the end of a doubly-linked list.
	Keeps track of current head and tail as you append. 
*/
int AppendList(WebPage *wpage, List *list) 
{
	struct ListNode *NewNode = malloc(sizeof(ListNode));
	NewNode->page = wpage;

	if (list->head == NULL) { //first node to be added to list
		list->head = NewNode;
		list->tail = NewNode;
		NewNode->prev = NULL;
		NewNode->next = NULL;
	}
	else {
		list->tail->next = NewNode;
		NewNode->prev = list->tail;
		list->tail = NewNode;
		NewNode->next = NULL;
	}
	return 1;
}

/* Returns the first WebPage from the List, shifts the head pointer to point to the 
	new head of the list. Frees the Listnode that contained the popped WebPage.
*/
WebPage * PopList(List *list)
{
	struct ListNode *temp = list->head;
	list->head = temp->next;
	struct WebPage *wpage = temp->page;
	free(temp);
	return wpage;
}


