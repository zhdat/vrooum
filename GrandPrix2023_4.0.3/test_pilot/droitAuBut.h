#ifndef DROITAUBUT_H
#define DROITAUBUT_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "follow_line.h"

#define HASH_SET_SIZE 1024

/* Structures utiles au projet */

/**
 * @brief Structure représentant un noeud dans l'espace de recherche
 *
 */
typedef struct Node {
	int x;
	int y;
	int speedX;
	int speedY;
	int gas;
	double g_cost;
	double h_cost;
	double f_cost;
	struct Node* parent;
} Node;

/**
 * @brief Structure représentant un élément dans une liste
 *
 */
typedef struct ListElement {
	void* data;
	struct ListElement* next;
} ListElement;

/**
 * @brief Structure représentant une liste
 *
 */
typedef struct List {
	ListElement* head;
} List;

typedef struct HashSetElement {
	Node* node;
	struct HashSetElement* next;
} HashSetElement;

typedef struct HashSet {
	HashSetElement* buckets[HASH_SET_SIZE];
} HashSet;

typedef struct PriorityQueueElement {
	Node* node;
	struct PriorityQueueElement* next;
} PriorityQueueElement;

typedef struct PriorityQueue {
	PriorityQueueElement* head;
} PriorityQueue;

/**
 * @brief Structure représentant une position finale
 *
 */
typedef struct {
	int x;
	int y;
	double distance;
} EndPosition;

typedef struct ArrayEnd {
	EndPosition* array;
	int size;
} ArrayEnd;

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions Hash */

HashSet* hsInit();

void hsInsert(HashSet* hs, Node* node);

int hsContains(const HashSet* hs, const Node* node);

void hsFree(HashSet* hs);

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions PriorityQueue */

PriorityQueue* pqInit();

void pqPush(PriorityQueue* pq, Node* node);

Node* pqPop(PriorityQueue* pq);

int pqIsEmpty(const PriorityQueue* pq);

void pqFree(PriorityQueue* pq);

Node* pqFind(PriorityQueue* pq, const Node* node);

void pqRemove(PriorityQueue* pq, const Node* node);

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions Noeuds */

/**
 * @brief Create a Node object
 *
 * @param x
 * @param y
 * @param parent
 * @param speedX
 * @param speedY
 * @param gas
 * @return Node* le noeud créé
 */
Node* createNode(int x, int y, Node* parent, int speedX, int speedY, int gas);

/**
 * @brief Vérifie l'égalité entre deux noeuds
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEquals(Node* node1, Node* node2);

/**
 * @brief Vérifie l'égalité entre deux noeuds sans la vitesse
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2);

/**
 * @brief Vérifie l'égalité entre deux noeuds sans la vitesse
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2);

/**
 * @brief Vérifie si un noeud est dans une liste
 *
 * @param node
 * @param list
 * @return int 1 si le noeud est dans la liste, 0 sinon
 */
int nodeInList(const Node* node, List* list);

/**
 * @brief Vérifie si un noeud est dans une liste et retourne l'élément de la liste
 *
 * @param node
 * @param list
 * @param elementInList
 * @return Node* le noeud si il est dans la liste, NULL sinon
 */
Node* findNodeInList(const Node* node, List* list, ListElement** elementInList);

/**
 * @brief Ajoute un noeud dans une liste
 *
 * @param node
 * @param list
 */
void addNodeToList(Node* node, List* list);

/**
 * @brief Supprime un noeud d'une liste
 *
 * @param list
 * @return Node* le noeud supprimé
 */
Node* removeNodeWithLowestFCost(List* list);

/**
 * @brief Libère la mémoire d'un noeud
 *
 * @param node
 */
void freeNode(Node* node);

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions list */

/**
 * @brief Initialise une liste
 *
 * @return List* la liste initialisée
 */
List* initList();

/**
 * @brief Vériie si une liste est vide
 *
 * @param list
 * @return int 1 si la liste est vide, 0 sinon
 */
int isListEmpty(const List* list);

/**
 * @brief Affiche le chemim
 *
 * @param path
 */
void printPath(List* path);

/**
 * @brief Inverse une liste
 *
 * @param list
 */
void reverseList(List* list);

void removeElementFromList(List* list, ListElement* element);

/**
 * @brief Libère la mémoire d'une liste
 *
 * @param path
 */
void freePath(List* path);

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions algorithme */

/**
 * @brief Calcule le coût heuristique
 *
 * @param a
 * @param b
 * @return int le coût heuristique
 */
double heuristicCost(Node* a, Node* b);

/**
 * @brief Vérifie si une position est occupée
 *
 * @param x
 * @param y
 * @param secondX
 * @param secondY
 * @param thirdX
 * @param thirdY
 * @return int 1 si la position est occupée, 0 sinon
 */
int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY);

/**
 * @brief Compare les positions finales
 *
 * @param a
 * @param b
 * @return int la différence entre les positions
 */
int compareEndPositions(const void* a, const void* b);

/**
 * @brief Vérifie si le chemin ne contient pas de mur
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @param end
 * @return int 1 si le chemin est libre, 0 sinon
 */
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end);

/**
 * @brief Compute the gas consumption of a requested acceleration
 *
 * CAUTION: Even an illegal move will result in gas consumption. Producing
 * illegal moves should be prevented as much as possible!
 *
 * @param accX Acceleration x component
 * @param accY Acceleration y component
 * @param speedX Speed x component
 * @param speedY Speed y component
 * @param inSand (boolean)
 * @return Number of gas units consumed
 */
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

/**
 * @brief Trouve les positions finales
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @param end
 * @param secondX
 * @param secondY
 * @param thirdX
 * @param thirdY
 * @param speedX
 * @param speedY
 */
ArrayEnd* findEndPositions(char** map, int width, int height, const Node* start);

/**
 * @brief Détermine l'accélération à partir du chemin
 *
 * @param path
 * @param myX
 * @param myY
 * @param accelerationX
 * @param accelerationY
 * @param speedX
 * @param speedY
 */
void determineAcceleration(const List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY);

/**
 * @brief Calcule la norme de la vitesse
 *
 * @param speedX
 * @param speedY
 * @return int la norme de la vitesse
 */
int SpeedNorme(int speedX, int speedY);

int shouldExploreNeighbor(Node* currentNode, char** map, int width, int height, int newX, int newY, int newSpeedX, int newSpeedY, Pos2Dint currentPos,
						  Pos2Dint newPos, int secondX, int secondY, int thirdX, int thirdY, int maxGas, int accX, int accY);

/**
 * @brief Calcule le chemin le plus court
 *
 * @param start
 * @param end
 * @param map
 * @param width
 * @param height
 * @param secondX
 * @param secondY
 * @param thirdX
 * @param thirdY
 * @param startSpeedX
 * @param startSpeedY
 * @param gasLevel
 * @return List* le chemin le plus court
 */
List* aStar(Node* start, Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY, int gasLevel,
			int currentSpeedX, int currentSpeedY, int speedMax);

#endif /* DROITAUBUT_H */