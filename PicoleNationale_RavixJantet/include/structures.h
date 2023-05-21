#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HASH_SET_SIZE 1024

/**
 * @brief The structure to store the end position of a car
 *
 */
typedef struct {
	int x;
	int y;
	double distance;
} EndPosition;

/**
 * @brief The structure to store an array of end positions
 *
 */
typedef struct ArrayEnd {
	EndPosition* array;
	int size;
} ArrayEnd;

/**
 * Position on a map (integer coordinates)
 */
typedef struct {
	int x;
	int y;
} Pos2Dint;

/**
 * Position with floating point coordinates
 */
typedef struct {
	float x;
	float y;
} Pos2Dfloat;

/**
 * Dicrete line traversal information
 */
typedef struct {
	Pos2Dint start;
	Pos2Dint end;
	Pos2Dfloat currentPosition;
	Pos2Dfloat delta; /*< Shift vector */
	int len;		  /*< Length along the main axis */
	int pos;		  /*< Reference position on the line */
} InfoLine;

/**
 * @brief The structure to store a linked list
 *
 */
typedef struct ListElement {
	void* data;
	struct ListElement* next;
} ListElement;

/**
 * @brief The structure to store a list
 *
 */
typedef struct List {
	ListElement* head;
} List;

/**
 * @brief The structure to store a node
 *
 */
typedef struct Node {
	int x;
	int y;
	int speedX;
	int speedY;
	int gas;
	int boostRemaining;
	double g_cost;
	double h_cost;
	double f_cost;
	struct Node* parent;
} Node;

/**
 * @brief The structure to store a hashset element
 *
 */
typedef struct HashSetElement {
	Node* node;
	struct HashSetElement* next;
} HashSetElement;

/**
 * @brief The structure to store a hashset
 *
 */
typedef struct HashSet {
	HashSetElement* buckets[HASH_SET_SIZE];
} HashSet;

/**
 * @brief The structure to store a priority queue element
 *
 */
typedef struct PriorityQueueElement {
	Node* node;
	struct PriorityQueueElement* next;
} PriorityQueueElement;

/**
 * @brief The structure to store a priority queue
 *
 */
typedef struct PriorityQueue {
	PriorityQueueElement* head;
} PriorityQueue;

#endif /* STRUCTURES_H */
