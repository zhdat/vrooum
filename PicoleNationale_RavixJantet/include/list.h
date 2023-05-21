#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

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
Node* createNode(int x, int y, Node* parent, int speedX, int speedY, int gas);

/**
 * @brief The function to check if a node is in the list
 *
 * @param node1
 * @param node2
 * @return int 1 if the nodes are equals, 0 otherwise
 */
int nodeEquals(const Node* node1, const Node* node2);

/**
 * @brief The function to check if a node is in the list without checking the speed
 *
 * @param node1
 * @param node2
 * @return int 1 if the nodes are equals, 0 otherwise
 */
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2);

/**
 * @brief The function to add a node to the list
 *
 * @param node
 * @param list
 */
void addNodeToList(Node* node, List* list);

/**
 * @brief The function to free a node
 *
 * @param node
 */
void freeNode(Node* node);

/**
 * @brief The function to create a new list
 *
 * @return List* The new list
 */
List* initList(void);

/**
 * @brief The function to verify if a list is empty
 *
 * @param list
 * @return int 1 if the list is empty, 0 otherwise
 */
int isListEmpty(const List* list);

/**
 * @brief The function to print a list
 *
 * @param path
 */
void printPath(List* path);

/**
 * @brief The function to reverse a list
 *
 * @param list
 */
void reverseList(List* list);

/**
 * @brief The function to free a list
 *
 * @param path
 */
void freePath(List* path);

#endif /* LIST_H */
