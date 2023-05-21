#ifndef HASHSET_H
#define HASHSET_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

unsigned int hashFunction(Node const* node);
HashSet* hsInit(void);
void hsInsert(HashSet* hs, Node* node);
int hsContains(const HashSet* hs, const Node* node);
void hsFree(HashSet* hs);
#endif /* HASHSET_H */
