#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

Node* createNode(int x, int y, Node* parent, int speedX, int speedY, int gas);
int nodeEquals(const Node* node1, const Node* node2);
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2);
void addNodeToList(Node* node, List* list);
void freeNode(Node* node);

List* initList(void);
int isListEmpty(const List* list);
void printPath(List* path);
void reverseList(List* list);
void freePath(List* path);

#endif /* LIST_H */
