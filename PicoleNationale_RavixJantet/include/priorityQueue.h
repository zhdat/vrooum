#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

PriorityQueue* pqInit(void);
void pqPush(PriorityQueue* pq, Node* node);
Node* pqPop(PriorityQueue* pq);
int pqIsEmpty(const PriorityQueue* pq);
void pqFree(PriorityQueue* pq);
Node* pqFind(PriorityQueue* pq, const Node* node);
void pqRemove(PriorityQueue* pq, const Node* node);

#endif /* PRIORITYQUEUE_H */
