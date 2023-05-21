#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/list.h"
#include "../include/hashSet.h"
#include "../include/follow_line.h"
#include "../include/priorityQueue.h"
#include "../include/aStar.h"

List* initList(void)
{
	List* newList = (List*)malloc(sizeof(List));
	newList->head = NULL;
	return newList;
}

int isListEmpty(const List* list)
{
	return list->head == NULL;
}

void printPath(List* path)
{
	ListElement* currentElement;
	Node const* currentNode;
	if (path == NULL || path->head == NULL) {
		fprintf(stderr, "Path is empty\n");
		return;
	}

	fprintf(stderr, "Path: ");
	currentElement = path->head;
	while (currentElement != NULL) {
		currentNode = (Node*)currentElement->data;
		fprintf(stderr, "(%d, %d) ", currentNode->x, currentNode->y);
		currentElement = currentElement->next;
	}
}

void reverseList(List* list)
{
	ListElement* prevElement;
	ListElement* currentElement;
	ListElement* nextElement;
	if (list == NULL) {
		return;
	}
	prevElement = NULL;
	currentElement = list->head;
	nextElement = NULL;

	if (list == NULL || list->head == NULL) {
		return;
	}

	while (currentElement != NULL) {
		nextElement = currentElement->next;
		currentElement->next = prevElement;
		prevElement = currentElement;
		currentElement = nextElement;
	}

	list->head = prevElement;
}

void freePath(List* path)
{
	ListElement* current;
	ListElement* tmp;
	if (path == NULL) {
		return;
	}
	current = path->head;
	while (current != NULL) {
		tmp = current;
		current = current->next;
		free(tmp);
	}
	free(path);
}
