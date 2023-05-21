#ifndef HASHSET_H
#define HASHSET_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

/**
 * @brief The function to calculate the hash of a node
 *
 * @param node
 * @return unsigned int  The hash of the node
 */
unsigned int hashFunction(Node const* node);

/**
 * @brief The function to create a new hashset
 *
 * @return HashSet* The new hashset
 */
HashSet* hsInit(void);

/**
 * @brief The function to insert a node in the hashset
 *
 * @param hs
 * @param node
 */
void hsInsert(HashSet* hs, Node* node);

/**
 * @brief The function to check if a node is in the hashset
 *
 * @param hs
 * @param node
 * @return int 1 if the node is in the hashset, 0 otherwise
 */
int hsContains(const HashSet* hs, const Node* node);

/**
 * @brief The function to free the hashset
 *
 * @param hs
 */
void hsFree(HashSet* hs);

#endif /* HASHSET_H */
