#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hashSet.h"
#include "../include/follow_line.h"
#include "../include/list.h"
#include "../include/priorityQueue.h"
#include "../include/aStar.h"

unsigned int hashFunction(Node const* node)
{
	return (node->x * 31 + node->y) % HASH_SET_SIZE;
}

HashSet* hsInit(void)
{
	int i;
	HashSet* hs = (HashSet*)malloc(sizeof(HashSet));
	for (i = 0; i < HASH_SET_SIZE; i++) {
		hs->buckets[i] = NULL;
	}
	return hs;
}

void hsInsert(HashSet* hs, Node* node)
{
	unsigned int hash = hashFunction(node);
	HashSetElement* newElement = (HashSetElement*)malloc(sizeof(HashSetElement));
	newElement->node = node;
	newElement->next = hs->buckets[hash];
	hs->buckets[hash] = newElement;
}

int hsContains(const HashSet* hs, const Node* node)
{
	unsigned int hash = hashFunction(node);
	HashSetElement* current = hs->buckets[hash];

	while (current != NULL) {
		if (nodeEqualsWithoutSpeed(current->node, node)) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

void hsFree(HashSet* hs)
{
	HashSetElement* current;
	HashSetElement* next;
	int i;

	for (i = 0; i < HASH_SET_SIZE; i++) {
		current = hs->buckets[i];

		while (current != NULL) {
			next = current->next;
			free(current);
			current = next;
		}
	}

	free(hs);
}
