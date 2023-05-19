#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "follow_line.h"
#include "droitAuBut.h"

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)
#define ULONG_MAX 4294967295

/* TESTS */

static unsigned int hashFunction(Node const* node)
{
	return (node->x * 31 + node->y) % HASH_SET_SIZE;
}

HashSet* hsInit()
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
		if (nodeEquals(current->node, node)) {
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

HashTable* createHashTable(int capacity)
{
	int i;
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	hashTable->capacity = capacity;
	hashTable->list = (HashNode**)malloc(sizeof(HashNode*) * capacity);
	for (i = 0; i < capacity; i++)
		hashTable->list[i] = NULL;
	return hashTable;
}

int hashCode(HashTable* ht, char* key)
{
	unsigned long int hashval = 0;
	int i = 0;
	while (hashval < ULONG_MAX && i < strlen(key)) {
		hashval = hashval << 8;
		hashval += key[i];
		i++;
	}
	return hashval % ht->capacity;
}

void insertToHash(HashTable* ht, char* key, int index)
{
	int pos = hashCode(ht, key);
	HashNode* list = ht->list[pos];
	HashNode* temp = list;
	while (temp) {
		if (strcmp(temp->key, key) == 0) {
			temp->index = index;
			return;
		}
		temp = temp->next;
	}
	HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
	newNode->key = strdup(key);
	newNode->index = index;
	newNode->next = list;
	ht->list[pos] = newNode;
}

void deleteFromHash(HashTable* ht, char* key)
{
	int pos = hashCode(ht, key);
	fprintf(stderr, "pos = %d\n", pos);
	HashNode* list = ht->list[pos];
	fprintf(stderr, "list = %p\n", list);
	HashNode *temp = list, *prev = NULL;
	fprintf(stderr, "temp = %p\n", temp);
	while (temp) {
		if (strcmp(temp->key, key) == 0) {
			if (temp == list) {
				ht->list[pos] = list->next;
			} else {
				prev->next = temp->next;
			}
			char* tempKey = temp->key;
			HashNode* tempNode = temp;
			temp = temp->next;
			free(tempKey);
			free(tempNode);
			return;
		}
		prev = temp;
		temp = temp->next;
	}
}

int getFromHash(HashTable* ht, char* key)
{
	int pos = hashCode(ht, key);
	HashNode* list = ht->list[pos];
	HashNode* temp = list;
	while (temp) {
		if (strcmp(temp->key, key) == 0)
			return temp->index;
		temp = temp->next;
	}
	return -1;
}

char* nodeHashKey(const Node* node)
{
	char* key = (char*)malloc(128 * sizeof(char));
	sprintf(key, "%d:%d", node->x, node->y);
	return key;
}

PriorityQueue* pqInit(int capacity)
{
	PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
	if (!pq)
		return NULL;
	pq->nodes = (Node**)malloc(sizeof(Node*) * capacity);
	if (!pq->nodes) {
		free(pq);
		return NULL;
	}
	pq->hashTable = createHashTable(capacity);
	pq->size = 0;
	pq->capacity = capacity;
	return pq;
}

void swap(Node** a, Node** b)
{
	Node* t = *a;
	*a = *b;
	*b = t;
}

void pqPush(PriorityQueue* pq, Node* node)
{
	if (pq->size == pq->capacity) {
		pq->capacity *= 2;
		Node** newNodes = (Node**)realloc(pq->nodes, sizeof(Node*) * pq->capacity);
		if (!newNodes) {
			return;
		}
		pq->nodes = newNodes;
	}

	int i = pq->size;
	pq->nodes[i] = node;
	char* key = nodeHashKey(node);
	insertToHash(pq->hashTable, key, i);
	free(key);
	pq->size++;

	while (i != 0 && pq->nodes[PARENT(i)]->f_cost > pq->nodes[i]->f_cost) {
		char* key1 = nodeHashKey(pq->nodes[i]);
		char* key2 = nodeHashKey(pq->nodes[PARENT(i)]);
		swap(&pq->nodes[i], &pq->nodes[PARENT(i)]);
		insertToHash(pq->hashTable, key1, PARENT(i));
		insertToHash(pq->hashTable, key2, i);
		free(key1);
		free(key2);
		i = PARENT(i);
	}
}

Node* pqPop(PriorityQueue* pq)
{
	if (pq->size == 0)
		return NULL;

	Node* root = pq->nodes[0];
	fprintf(stderr, "pqPop: %d:%d\n", root->x, root->y);
	char* key = nodeHashKey(root);
	fprintf(stderr, "pqPop: %s\n", key);
	deleteFromHash(pq->hashTable, key);
	fprintf(stderr, "pqPop: %s\n", key);
	free(key);

	pq->size--;

	if (pq->size > 0) {
		pq->nodes[0] = pq->nodes[pq->size];
		key = nodeHashKey(pq->nodes[0]);
		insertToHash(pq->hashTable, key, 0);
		free(key);

		MinHeapify(pq, 0);
	}

	return root;
}

void MinHeapify(PriorityQueue* pq, int i)
{
	int left = LEFT(i);
	int right = RIGHT(i);
	int smallest = i;

	if (left < pq->size && pq->nodes[left]->f_cost < pq->nodes[i]->f_cost)
		smallest = left;

	if (right < pq->size && pq->nodes[right]->f_cost < pq->nodes[smallest]->f_cost)
		smallest = right;

	if (smallest != i) {
		char* key1 = nodeHashKey(pq->nodes[i]);
		char* key2 = nodeHashKey(pq->nodes[smallest]);

		swap(&pq->nodes[i], &pq->nodes[smallest]);

		insertToHash(pq->hashTable, key1, smallest);
		insertToHash(pq->hashTable, key2, i);

		free(key1);
		free(key2);

		MinHeapify(pq, smallest);
	}
}

int pqIsEmpty(const PriorityQueue* pq)
{
	return pq->size == 0;
}

void pqFree(PriorityQueue* pq)
{
	int i;
	if (pq) {
		if (pq->nodes) {
			free(pq->nodes);
		}

		if (pq->hashTable) {
			for (i = 0; i < pq->hashTable->capacity; i++) {
				HashNode* node = pq->hashTable->list[i];
				while (node != NULL) {
					HashNode* temp = node;
					node = node->next;
					free(temp->key);
					free(temp);
				}
			}
			free(pq->hashTable->list);
			free(pq->hashTable);
		}

		free(pq);
	}
}

Node* pqFind(PriorityQueue* pq, const Node* node)
{
	char* key = nodeHashKey(node);
	int index = getFromHash(pq->hashTable, key);
	free(key);
	if (index != -1)
		return pq->nodes[index];
	else
		return NULL;
}

void pqRemove(PriorityQueue* pq, const Node* node)
{
	char* key = nodeHashKey(node);
	int index = getFromHash(pq->hashTable, key);
	if (index != -1) {
		deleteFromHash(pq->hashTable, key);
		swap(&pq->nodes[index], &pq->nodes[pq->size - 1]);
		pq->size--;
		MinHeapify(pq, index);
	}
	free(key);
}

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
Node* createNode(int x, int y, Node* parent, int speedX, int speedY, int gas)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->x = x;
	newNode->y = y;
	newNode->parent = parent;
	newNode->speedX = speedX;
	newNode->speedY = speedY;
	newNode->gas = gas;
	newNode->g_cost = 0;
	newNode->h_cost = 0;
	newNode->f_cost = 0;
	return newNode;
}

/**
 * @brief Vérifie l'égalité entre deux noeuds
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEquals(const Node* node1, const Node* node2)
{
	if (node1 == NULL || node2 == NULL) {
		return 0;
	}
	return node1->x == node2->x && node1->y == node2->y && node1->speedX == node2->speedX && node1->speedY == node2->speedY &&
		   node1->gas == node2->gas;
}

/**
 * @brief Vérifie l'égalité entre deux noeuds sans la vitesse
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEqualsWithoutSpeed(const Node* node1, const Node* node2)
{
	if (node1 == NULL || node2 == NULL) {
		return 0;
	}
	return node1->x == node2->x && node1->y == node2->y;
}

/**
 * @brief Vérifie si un noeud est dans une liste
 *
 * @param node
 * @param list
 * @return int 1 si le noeud est dans la liste, 0 sinon
 */
int nodeInList(const Node* node, List* list)
{
	ListElement* current = list->head;
	while (current != NULL) {
		Node const* currentNode = (Node*)current->data;
		if (nodeEqualsWithoutSpeed(currentNode, node)) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/**
 * @brief Vérifie si un noeud est dans une liste et retourne l'élément de la liste
 *
 * @param node
 * @param list
 * @param elementInList
 * @return Node* le noeud si il est dans la liste, NULL sinon
 */
Node* findNodeInList(const Node* node, List* list, ListElement** elementInList)
{
	ListElement* currentElement = list->head;

	while (currentElement != NULL) {
		Node* currentNode = (Node*)currentElement->data;

		if (nodeEquals(currentNode, node)) {
			if (elementInList != NULL) {
				*elementInList = currentElement;
			}
			return currentNode;
		}

		currentElement = currentElement->next;
	}

	return NULL;
}

/**
 * @brief Ajoute un noeud dans une liste
 *
 * @param node
 * @param list
 */
void addNodeToList(Node* node, List* list)
{
	ListElement* newElement = (ListElement*)malloc(sizeof(ListElement));
	newElement->data = node;
	newElement->next = NULL;

	if (list->head == NULL) {
		list->head = newElement;
	} else {
		ListElement* current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newElement;
	}
}

/**
 * @brief Supprime un noeud d'une liste
 *
 * @param list
 * @return Node* le noeud supprimé
 */
Node* removeNodeWithLowestFCost(List* list)
{
	Node* result;
	ListElement* current = list->head;
	ListElement* previous = NULL;
	ListElement* lowest = current;
	ListElement* previousLowest = NULL;

	double lowestCost = ((Node*)current->data)->f_cost;

	while (current != NULL) {
		Node const* currentNode = (Node*)current->data;
		if (currentNode->f_cost < lowestCost) {
			lowestCost = currentNode->f_cost;
			lowest = current;
			previousLowest = previous;
		}
		previous = current;
		current = current->next;
	}

	if (previousLowest == NULL) {
		list->head = lowest->next;
	} else {
		previousLowest->next = lowest->next;
	}

	result = (Node*)lowest->data;
	free(lowest);
	return result;
}

/**
 * @brief Libère la mémoire d'un noeud
 *
 * @param node
 */
void freeNode(Node* node)
{
	if (node != NULL) {
		free(node);
	}
}

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions List */

/**
 * @brief Initialise une liste
 *
 * @return List* la liste initialisée
 */
List* initList()
{
	List* newList = (List*)malloc(sizeof(List));
	newList->head = NULL;
	return newList;
}

/**
 * @brief Vériie si une liste est vide
 *
 * @param list
 * @return int 1 si la liste est vide, 0 sinon
 */
int isListEmpty(const List* list)
{
	return list->head == NULL;
}

/**
 * @brief Affiche le chemim
 *
 * @param path
 */
void printPath(List* path)
{
	ListElement* currentElement;
	Node const* currentNode;
	if (path == NULL || path->head == NULL) {
		fprintf(stderr, "Path is empty\n");
		return;
	}

	fprintf(stderr, "Path: ");
	currentElement = path->head;
	while (currentElement != NULL) {
		currentNode = (Node*)currentElement->data;
		fprintf(stderr, "(%d, %d) ", currentNode->x, currentNode->y);
		currentElement = currentElement->next;
	}
}

/**
 * @brief Inverse une liste
 *
 * @param list
 */
void reverseList(List* list)
{
	ListElement* prevElement;
	ListElement* currentElement;
	ListElement* nextElement;
	if (list == NULL) {
		return;
	}
	prevElement = NULL;
	currentElement = list->head;
	nextElement = NULL;

	if (list == NULL || list->head == NULL) {
		return;
	}

	while (currentElement != NULL) {
		nextElement = currentElement->next;
		currentElement->next = prevElement;
		prevElement = currentElement;
		currentElement = nextElement;
	}

	list->head = prevElement;
}

void removeElementFromList(List* list, ListElement* element)
{
	if (element == list->head) {
		list->head = element->next;
	} else {
		ListElement* previous = list->head;
		while (previous->next != element) {
			previous = previous->next;
		}
		previous->next = element->next;
	}
	free(element);
}

/**
 * @brief Libère la mémoire d'une liste
 *
 * @param path
 */
void freePath(List* path)
{
	ListElement* current;
	ListElement* tmp;
	if (path == NULL) {
		return;
	}
	current = path->head;
	while (current != NULL) {
		tmp = current;
		current = current->next;
		free(tmp);
	}
	free(path);
}

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonctions algorithme */

/**
 * @brief Calcule le coût heuristique
 *
 * @param a
 * @param b
 * @return int le coût heuristique
 */
double heuristicCost(const Node* a, const Node* b)
{
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

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
int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY)
{
	return (x == secondX && y == secondY) || (x == thirdX && y == thirdY);
}

/**
 * @brief Compare les positions finales
 *
 * @param a
 * @param b
 * @return int la différence entre les positions
 */
int compareEndPositions(const void* a, const void* b)
{
	const EndPosition* positionA = (const EndPosition*)a;
	const EndPosition* positionB = (const EndPosition*)b;

	if (positionA->distance < positionB->distance)
		return -1;
	else if (positionA->distance > positionB->distance)
		return 1;
	else
		return 0;
}

/**
 * @brief Vérifie si le chemin ne contient pas de mur entre deux positions.
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @param end
 * @return int 1 si le chemin est libre, 0 sinon
 */
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end, int secondX, int secondY, int thirdX, int thirdY)
{
	InfoLine vline;
	Pos2Dint p;
	initLine(start.x, start.y, end.x, end.y, &vline);
	while (nextPoint(&vline, &p, +1) > 0) {
		if (p.x == start.x && p.y == start.y) {
			/* We suppose that the start position is not worth visiting! */
			continue;
		}
		if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
			/* We suppose that the map is surrounded by walls! */
			return 0;
		}
		if (map[p.y][p.x] == '.') {
			return 0;
		}
		if (map[p.y][p.x] == '1') {
			return 0;
		}
		if (map[p.y][p.x] == '2') {
			return 0;
		}
		if (map[p.y][p.x] == '3') {
			return 0;
		}
	}
	return 1; /*Path is clear*/
}

int isPathClear_Occupied(char** map, int width, int height, Pos2Dint start, Pos2Dint end, int secondX, int secondY, int thirdX, int thirdY)
{
	InfoLine vline;
	Pos2Dint p;
	initLine(start.x, start.y, end.x, end.y, &vline);
	while (nextPoint(&vline, &p, +1) > 0) {
		if (p.x == start.x && p.y == start.y) {
			/* We suppose that the start position is not worth visiting! */
			continue;
		}
		if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
			/* We suppose that the map is surrounded by walls! */
			return 0;
		}
		if (map[p.y][p.x] == '.') {
			return 0;
		}
		if (map[p.y][p.x] == '1') {
			return 0;
		}
		if (map[p.y][p.x] == '2') {
			return 0;
		}
		if (map[p.y][p.x] == '3') {
			return 0;
		}
		if (p.y == secondY && p.x == secondX) {
			return 0;
		}
		if (p.y == thirdY && p.x == thirdX) {
			return 0;
		}
	}
	return 1; /*Path is clear*/
}

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
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand)
{
	int gas = accX * accX + accY * accY;
	gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
	if (inSand) {
		gas += 1;
	}
	return -gas;
}

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
ArrayEnd* findEndPositions(char** map, int width, int height, const Node* start)
{
	int x;
	int y;
	double distance;
	EndPosition endPosition;
	ArrayEnd* arrayEnd;

	int endPositionCount = 0;
	arrayEnd = (ArrayEnd*)malloc(sizeof(ArrayEnd));
	arrayEnd->array = (EndPosition*)malloc(sizeof(EndPosition));

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (map[y][x] == '=') {
				Node node;
				node.x = x;
				node.y = y;
				distance = heuristicCost(start, &node);
				endPosition.x = x;
				endPosition.y = y;
				endPosition.distance = distance;
				arrayEnd->array[endPositionCount++] = endPosition;
				arrayEnd->array = (EndPosition*)realloc(arrayEnd->array, sizeof(EndPosition) * (endPositionCount + 1));
			}
		}
	}
	arrayEnd->size = endPositionCount;
	return arrayEnd;
}

void findBestEnd(int myX, int myY, int secondX, int secondY, int thirdX, int thirdY, int speedX, int speedY, ArrayEnd* array, Node** end)
{
	int j;
	int i;

	for (i = 0; i < array->size; i++) {
		array->array[i].distance =
			sqrt((array->array[i].x - myX) * (array->array[i].x - myX) + (array->array[i].y - myY) * (array->array[i].y - myY));
	}

	qsort(array->array, array->size, sizeof(EndPosition), compareEndPositions);
	*end = createNode(array->array[0].x, array->array[0].y, NULL, speedX, speedY, 0);

	for (i = 0; i < array->size; i++) {
		int x = array->array[i].x;
		int y = array->array[i].y;
		if (isPositionOccupied(x, y, secondX, secondY, thirdX, thirdY) == 0) {
			*end = createNode(x, y, NULL, speedX, speedY, 0);
			break;
		}
	}
}

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
void determineAcceleration(const List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY, char** map,
						   int* boosts)
{
	int nextX;
	int nextY;
	Node const* first;
	if (path == NULL || path->head == NULL || path->head->data == NULL) {
		*accelerationX = 0;
		*accelerationY = 0;
		return;
	}
	first = path->head->next->data;
	nextX = first->x;
	nextY = first->y;

	/* Vérifier si la vitesse actuelle est suffisante pour atteindre la case suivante */
	if (myX + speedX == nextX && myY + speedY == nextY) {
		*accelerationX = 0;
		*accelerationY = 0;
	} else {
		int desiredSpeedX = nextX - myX;
		int desiredSpeedY = nextY - myY;

		*accelerationX = desiredSpeedX - speedX;
		*accelerationY = desiredSpeedY - speedY;
	}

	/* Vérifier si l'accélération est trop grande */
	if (*accelerationX > 1) {
		*accelerationX = 1;
	}
	if (*accelerationX < -1) {
		*accelerationX = -1;
	}
	if (*accelerationY > 1) {
		*accelerationY = 1;
	}
	if (*accelerationY < -1) {
		*accelerationY = -1;
	}

	/* if (*boosts > 0) {
		if (*boosts != 0) {
			if (SpeedNorme(speedX + (*accelerationX * 2), speedY + (*accelerationY * 2)) <= 25) {
				*accelerationX = *accelerationX * 2;
				*accelerationY = *accelerationY * 2;
				*boosts--;
			}
		}
	} */

	/* Accélération dans le sable */
	if (map[myY][myX] == '~') {
		if (SpeedNorme(speedX + *accelerationX, speedY + *accelerationY) > 1) {
			*accelerationX = 0;
			*accelerationY = 0;
		}
	}
}

/**
 * @brief Calcule la norme de la vitesse
 *
 * @param speedX
 * @param speedY
 * @return int la norme de la vitesse
 */
int SpeedNorme(int speedX, int speedY)
{
	return (speedX * speedX + speedY * speedY);
}

int shouldContinue(int newX, int newY, int width, int height, char** map, int currentNodeX, int currentNodeY, int accX, int accY, int secondX,
				   int secondY, int thirdX, int thirdY, int newSpeedX, int newSpeedY)
{
	if (newX == currentNodeX && newY == currentNodeY) {
		return 0; /* ignore the current node */
	}

	if (newX >= width || newY >= height || newX < 0 || newY < 0) {
		return 0; /* out of bounds */
	}

	if (map[newY][newX] == '.') {
		return 0; /* obstacle */
	}

	if ((map[currentNodeY][currentNodeX] == '~') && (accX != 0 && accY != 0)) {
		return 0;
	}

	return 1; /* continue with current iteration */
}

Node* createNeighbourNode(int newX, int newY, Node* currentNode, int newSpeedX, int newSpeedY, int newGas, char** map, const Node* end)
{
	int factor;
	Node* neighbour = createNode(newX, newY, currentNode, newSpeedX, newSpeedY, newGas);
	factor = 1;
	if (map[newY][newX] == '~') {
		factor += 4;
	}
	if (SpeedNorme(newSpeedX, newSpeedY) < 25) {
		factor += 2;
	}
	neighbour->g_cost =
		currentNode->g_cost + factor * sqrt((currentNode->x - newX) * (currentNode->x - newX) + (currentNode->y - newY) * (currentNode->y - newY));

	neighbour->h_cost = heuristicCost(neighbour, end);
	neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;
	return neighbour;
}

List* getPath(Node* currentNode)
{
	List* path = initList();
	Node* pathNode = currentNode;
	while (pathNode != NULL) {
		addNodeToList(pathNode, path);
		pathNode = pathNode->parent;
	}
	return path;
}

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
List* aStar(Node* start, const Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY, int gasLevel,
			int currentSpeedX, int currentSpeedY, int speedMax, int maxGas, int occupied, int occupiedX, int occupiedY)
{
	int accX;
	int accY;
	int newSpeedX;
	int newSpeedY;
	int newX;
	int newY;
	int newGas;
	Node* neighbour;

	PriorityQueue* openSet = pqInit(10);
	HashSet* closedSet = hsInit();
	fprintf(stderr, "start : %d %d\n", start->x, start->y);

	start->g_cost = 0;
	start->h_cost = heuristicCost(start, end);
	start->f_cost = start->g_cost + start->h_cost;
	start->gas = gasLevel;
	start->speedX = currentSpeedX;
	start->speedY = currentSpeedY;

	pqPush(openSet, start);
	fprintf(stderr, "end : %d %d\n", end->x, end->y);

	while (!pqIsEmpty(openSet)) {
		fprintf(stderr, "openSet : %d\n", openSet->size);
		Node* currentNode = pqPop(openSet);
		fprintf(stderr, "current : %d %d\n", currentNode->x, currentNode->y);

		if (nodeEqualsWithoutSpeed(currentNode, end) == 1) {
			List* path;
			path = getPath(currentNode);
			return path;
		}
		fprintf(stderr, "current : %d %d\n", currentNode->x, currentNode->y);
		hsInsert(closedSet, currentNode);
		fprintf(stderr, "current : %d %d\n", currentNode->x, currentNode->y);

		/* Générer les voisins */
		for (accX = -1; accX <= 1; accX++) {
			for (accY = -1; accY <= 1; accY++) {
				newSpeedX = currentNode->speedX + accX;
				newSpeedY = currentNode->speedY + accY;

				if ((newSpeedX * newSpeedX) + (newSpeedY * newSpeedY) <= speedMax) {
					newX = currentNode->x + newSpeedX;
					newY = currentNode->y + newSpeedY;

					if (shouldContinue(newX, newY, width, height, map, currentNode->x, currentNode->y, accX, accY, secondX, secondY, thirdX, thirdY,
									   0, 0) == 0) {
						continue;
					}

					Pos2Dint currentPos;
					Pos2Dint newPos;

					currentPos.x = currentNode->x;
					currentPos.y = currentNode->y;
					newPos.x = newX;
					newPos.y = newY;

					if (occupied == 1) {
						if ((newX == secondX && newY == secondY) || (newX == thirdX && newY == thirdY) ||
							isPathClear_Occupied(map, width, height, currentPos, newPos, secondX, secondY, thirdX, thirdY) == 0) {
							continue;
						}
					}

					if (isPathClear(map, width, height, currentPos, newPos, secondX, secondY, thirdX, thirdY) == 0)
						continue;

					newGas = currentNode->gas + gasConsumption(accX, accY, newSpeedX, newSpeedY, map[newY][newX] == '~');
					if (newGas < 0)
						continue;

					neighbour = createNeighbourNode(newX, newY, currentNode, newSpeedX, newSpeedY, newGas, map, end);

					if (!hsContains(closedSet, neighbour)) {
						Node const* existingNodeInOpenSet = pqFind(openSet, neighbour);
						if (existingNodeInOpenSet == NULL || neighbour->g_cost < existingNodeInOpenSet->g_cost) {
							if (existingNodeInOpenSet != NULL) {
								pqRemove(openSet, existingNodeInOpenSet);
							}
							pqPush(openSet, neighbour);
						}
					}
				}
			}
		}
	}

	/* Pas de chemin trouvé */
	return NULL;
}

/* ------------------------------------------------------------------------------------------------------------------ */

/* Fonction Main() */
int main()
{
	int row;
	int width;
	int height;
	int gasLevel;
	int maxGas;
	int* boosts = BOOSTS_AT_START;
	int round = 0;
	int accelerationX = 0;
	int accelerationY = 0;
	int speedX = 0;
	int speedY = 0;
	char action[100];
	char line_buffer[MAX_LINE_LENGTH];
	char** map;
	int myX;
	int myY;
	int secondX;
	int secondY;
	int thirdX;
	int thirdY;
	int i;
	int vitesse = 25;
	int oldX;
	int oldY;
	int occupied;
	int occupiedX = 0;
	int occupiedY = 0;

	ArrayEnd* arrayEnd = NULL;
	Node* start = NULL;
	Node* end = NULL;
	List* path = NULL;
	occupied = 0;

	boosts = boosts;							/* Prevent warning "unused variable" */
	fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
	sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
	maxGas = gasLevel;
	fprintf(stderr, "=== >Map< ===\n");
	fprintf(stderr, "Size %d x %d\n", width, height);
	fprintf(stderr, "Gas at start %d \n\n", gasLevel);
	map = (char**)malloc(height * sizeof(char*));
	for (row = 0; row < height; ++row) { /* Read map data, line per line */
		fgets(line_buffer, MAX_LINE_LENGTH, stdin);
		fputs(line_buffer, stderr);
		map[row] = (char*)malloc(width * sizeof(char));
		strcpy(map[row], line_buffer);
		map[row][width] = '\0'; /* Remove trailing \n */
	}
	fflush(stderr);
	fprintf(stderr, "\n=== Race start ===\n");

	while (!feof(stdin)) {
		vitesse = 25;
		i = 1;
		occupied = 0;
		round++;

		fprintf(stderr, "=== ROUND %d\n", round);
		fflush(stderr);
		fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots */
		sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
		fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n", myX, myY, secondX, secondY, thirdX, thirdY);
		fflush(stderr);

		if (round != 1 && oldX == myX && oldY == myY) {
			speedX = 0;
			speedY = 0;
		}
		oldX = myX;
		oldY = myY;

		if (round == 1) {
			/* Trouver les positions de départ et d'arrivée sur la carte */
			start = createNode(myX, myY, NULL, speedX, speedY, maxGas);
			arrayEnd = findEndPositions(map, width, height, start);
		}

		start->x = myX;
		start->y = myY;

		findBestEnd(myX, myY, secondX, secondY, thirdX, thirdY, speedX, speedY, arrayEnd, &end);
		fprintf(stderr, "    End: (%d,%d)\n", end->x, end->y);
		/* Executer l'algorithme A* pour trouver le chemin */
		path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, maxGas, occupied, occupiedX,
					 occupiedY);
		fprintf(stderr, "    Path: ");
		reverseList(path);
		printPath(path);

		if (path != NULL) {
			Node* firstNode;
			Pos2Dint firstNodePos;
			Pos2Dint startNodePos;
			firstNode = path->head->next->data;
			firstNodePos.x = firstNode->x;
			firstNodePos.y = firstNode->y;
			startNodePos.x = start->x;
			startNodePos.y = start->y;

			if ((firstNode->x == secondX && firstNode->y == secondY) || (firstNode->x == thirdX && firstNode->y == thirdY) ||
				isPathClear_Occupied(map, width, height, startNodePos, firstNodePos, secondX, secondY, thirdX, thirdY) == 0) {
				vitesse = 25;
				occupied = 1;
				occupiedX = firstNode->x;
				occupiedY = firstNode->y;
				path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, maxGas, occupied,
							 occupiedX, occupiedY);
				reverseList(path);
				printPath(path);
			}
		}

		/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
		determineAcceleration(path, myX, myY, &accelerationX, &accelerationY, speedX, speedY, map, &boosts);

		/* Gas consumption cannot be accurate here. */
		gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, map[myY][myX] == '~');
		speedX += accelerationX;
		speedY += accelerationY;

		/* Write the acceleration request to the race manager (stdout). */
		sprintf(action, "%d %d", accelerationX, accelerationY);
		fprintf(stdout, "%s", action);
		fflush(stdout); /* CAUTION : This is necessary  */
		fprintf(stderr, "    Action: %s   Gas remaining: %d\n", action, gasLevel);
		fflush(stderr);

		if (0 && round > 4) { /* (DISABLED) Force a segfault for testing purpose */
			int* p = NULL;
			fprintf(stderr, "Good Bye!\n");
			fflush(stderr);
			*p = 0;
		}
	}

	/* Nettoyage des ressources allouées */
	for (row = 0; row < height; ++row) {
		free(map[row]);
	}
	free(map);
	freePath(path);
	freeNode(start);
	freeNode(end);

	return EXIT_SUCCESS;
}
