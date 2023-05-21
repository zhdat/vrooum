#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hashSet.h"
#include "../include/follow_line.h"
#include "../include/list.h"
#include "../include/priorityQueue.h"
#include "../include/aStar.h"

/**
 * @brief The function to calculate the hash of a node
 *
 * @param node
 * @return unsigned int  The hash of the node
 */
unsigned int hashFunction(Node const* node)
{
	return (node->x * 31 + node->y) % HASH_SET_SIZE;
}

/**
 * @brief The function to create a new hashset
 *
 * @return HashSet* The new hashset
 */
HashSet* hsInit(void)
{
	int i;
	HashSet* hs = (HashSet*)malloc(sizeof(HashSet));
	for (i = 0; i < HASH_SET_SIZE; i++) {
		hs->buckets[i] = NULL;
	}
	return hs;
}

/**
 * @brief The function to insert a node in the hashset
 *
 * @param hs
 * @param node
 */
void hsInsert(HashSet* hs, Node* node)
{
	unsigned int hash = hashFunction(node);
	HashSetElement* newElement = (HashSetElement*)malloc(sizeof(HashSetElement));
	newElement->node = node;
	newElement->next = hs->buckets[hash];
	hs->buckets[hash] = newElement;
}

/**
 * @brief The function to check if a node is in the hashset
 *
 * @param hs
 * @param node
 * @return int 1 if the node is in the hashset, 0 otherwise
 */
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

/**
 * @brief The function to free the hashset
 *
 * @param hs
 */
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
