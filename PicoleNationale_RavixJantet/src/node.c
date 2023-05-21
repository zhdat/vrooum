#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/list.h"
#include "../include/hashSet.h"
#include "../include/follow_line.h"
#include "../include/priorityQueue.h"
#include "../include/aStar.h"

#define BOOSTS_AT_START 5

/**
 * @brief Create a Node object
 *
 * @param x
 * @param y
 * @param parent
 * @param speedX
 * @param speedY
 * @param gas
 * @return Node*
 */
Node* createNode(int x, int y, Node* parent, int speedX, int speedY, int gas)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->x = x;
	newNode->y = y;
	newNode->parent = parent;
	newNode->speedX = speedX;
	newNode->speedY = speedY;
	newNode->boostRemaining = BOOSTS_AT_START;
	newNode->gas = gas;
	newNode->g_cost = 0;
	newNode->h_cost = 0;
	newNode->f_cost = 0;
	return newNode;
}

/**
 * @brief The function to check if a node is in the list
 *
 * @param node1
 * @param node2
 * @return int 1 if the nodes are equals, 0 otherwise
 */
int nodeEquals(const Node* node1, const Node* node2)
{
	if (node1 == NULL || node2 == NULL) {
		return 0;
	}
	return node1->x == node2->x && node1->y == node2->y && node1->speedX == node2->speedX && node1->speedY == node2->speedY &&
		   node1->gas == node2->gas && node1->boostRemaining == node2->boostRemaining;
}

/**
 * @brief The function to check if a node is in the list without checking the speed
 *
 * @param node1
 * @param node2
 * @return int 1 if the nodes are equals, 0 otherwise
 */
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2)
{
	if (node1 == NULL || node2 == NULL) {
		return 0;
	}
	return node1->x == node2->x && node1->y == node2->y;
}

/**
 * @brief The function to add a node to the list
 *
 * @param node
 * @param list
 */
void addNodeToList(Node* node, List* list)
{
	ListElement* newElement = (ListElement*)malloc(sizeof(ListElement));
	newElement->data = node;
	newElement->next = NULL;

	if (list->head == NULL) {
		list->head = newElement;
	} else {
		ListElement* current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newElement;
	}
}

/**
 * @brief The function to free a node
 *
 * @param node
 */
void freeNode(Node* node)
{
	if (node != NULL) {
		free(node);
	}
}
