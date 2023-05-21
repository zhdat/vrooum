#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

/**
 * @brief The function to create a new priority queue
 *
 * @return PriorityQueue* The new priority queue
 */
PriorityQueue* pqInit(void);

/**
 * @brief The function to push a node in the priority queue
 *
 * @param pq
 * @param node
 */
void pqPush(PriorityQueue* pq, Node* node);

/**
 * @brief The function to pop the first node of the priority queue
 *
 * @param pq
 * @return Node* The node popped
 */
Node* pqPop(PriorityQueue* pq);

/**
 * @brief The function to check if the priority queue is empty
 *
 * @param pq
 * @return int 1 if the priority queue is empty, 0 otherwise
 */
int pqIsEmpty(const PriorityQueue* pq);

/**
 * @brief The function to free the priority queue
 *
 * @param pq
 */
void pqFree(PriorityQueue* pq);

/**
 * @brief The function to find a node in the priority queue
 *
 * @param pq
 * @param node
 * @return Node*
 */
Node* pqFind(PriorityQueue* pq, const Node* node);

/**
 * @brief The function to remove a node from the priority queue
 *
 * @param pq
 * @param node
 */
void pqRemove(PriorityQueue* pq, const Node* node);

#endif /* PRIORITYQUEUE_H */
