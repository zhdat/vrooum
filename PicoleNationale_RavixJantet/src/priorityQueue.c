#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/priorityQueue.h"
#include "../include/hashSet.h"
#include "../include/follow_line.h"
#include "../include/list.h"
#include "../include/aStar.h"

/**
 * @brief The function to create a new priority queue
 *
 * @return PriorityQueue* The new priority queue
 */
PriorityQueue* pqInit(void)
{
	PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
	pq->head = NULL;
	return pq;
}

/**
 * @brief The function to push a node in the priority queue
 *
 * @param pq
 * @param node
 */
void pqPush(PriorityQueue* pq, Node* node)
{
	PriorityQueueElement* current;
	PriorityQueueElement* newElement = (PriorityQueueElement*)malloc(sizeof(PriorityQueueElement));
	newElement->node = node;
	newElement->next = NULL;

	if (pq->head == NULL || pq->head->node->f_cost > node->f_cost) {
		newElement->next = pq->head;
		pq->head = newElement;
		return;
	}

	current = pq->head;
	while (current->next != NULL && current->next->node->f_cost < node->f_cost) {
		current = current->next;
	}
	newElement->next = current->next;
	current->next = newElement;
}

/**
 * @brief The function to pop the first node of the priority queue
 *
 * @param pq
 * @return Node* The node popped
 */
Node* pqPop(PriorityQueue* pq)
{
	PriorityQueueElement* elementToRemove;
	Node* node;
	if (pq->head == NULL) {
		return NULL;
	}
	elementToRemove = pq->head;
	node = elementToRemove->node;
	pq->head = elementToRemove->next;
	free(elementToRemove);
	return node;
}

/**
 * @brief The function to check if the priority queue is empty
 *
 * @param pq
 * @return int 1 if the priority queue is empty, 0 otherwise
 */
int pqIsEmpty(const PriorityQueue* pq)
{
	return pq->head == NULL;
}

/**
 * @brief The function to free the priority queue
 *
 * @param pq
 */
void pqFree(PriorityQueue* pq)
{
	PriorityQueueElement* current = pq->head;
	PriorityQueueElement* next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	free(pq);
}

/**
 * @brief The function to find a node in the priority queue
 *
 * @param pq
 * @param node
 * @return Node*
 */
Node* pqFind(PriorityQueue* pq, const Node* node)
{
	PriorityQueueElement* current = pq->head;
	while (current != NULL) {
		if (nodeEqualsWithoutSpeed(current->node, node)) {
			return current->node;
		}
		current = current->next;
	}
	return NULL;
}

/**
 * @brief The function to remove a node from the priority queue
 *
 * @param pq
 * @param node
 */
void pqRemove(PriorityQueue* pq, const Node* node)
{
	PriorityQueueElement* current;
	if (pq->head == NULL) {
		return;
	}
	if (nodeEqualsWithoutSpeed(pq->head->node, node)) {
		PriorityQueueElement* elementToRemove = pq->head;
		pq->head = pq->head->next;
		free(elementToRemove);
		return;
	}
	current = pq->head;
	while (current->next != NULL) {
		if (nodeEqualsWithoutSpeed(current->next->node, node)) {
			PriorityQueueElement* elementToRemove = current->next;
			current->next = current->next->next;
			free(elementToRemove);
			return;
		}
		current = current->next;
	}
}
