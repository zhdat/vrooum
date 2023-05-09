#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "follow_line.h"
#include "droitAuBut.h"
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5
#define INFINITE INT_MAX

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
int nodeEquals(Node* node1, Node* node2)
{
	return node1->x == node2->x && node1->y == node2->y && node1->speedX == node2->speedX && node1->speedY == node2->speedY;
}

/**
 * @brief Vérifie l'égalité entre deux noeuds sans la vitesse
 *
 * @param node1
 * @param node2
 * @return int 1 si les noeuds sont égaux, 0 sinon
 */
int nodeEqualsWithoutSpeed(Node* node1, Node* node2)
{
	return node1->x == node2->x && node1->y == node2->y;
}

/**
 * @brief Vérifie si un noeud est dans une liste
 *
 * @param node
 * @param list
 * @return int 1 si le noeud est dans la liste, 0 sinon
 */
int nodeInList(Node* node, List* list)
{
	ListElement* current = list->head;
	while (current != NULL) {
		Node* current_node = (Node*)current->data;
		if (nodeEqualsWithoutSpeed(current_node, node)) {
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
Node* findNodeInList(Node* node, List* list, ListElement** elementInList)
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

	int lowestCost = ((Node*)current->data)->f_cost;

	while (current != NULL) {
		Node* current_node = (Node*)current->data;
		if (current_node->f_cost < lowestCost) {
			lowestCost = current_node->f_cost;
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
int isListEmpty(List* list)
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
	Node* currentNode;
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
		fprintf(stderr, "List is NULL\n");
		return;
	}
	prevElement = NULL;
	currentElement = list->head;
	nextElement = NULL;

	if (list == NULL || list->head == NULL) {
		fprintf(stderr, "List is empty\n");
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

/**
 * @brief Libère la mémoire d'une liste
 *
 * @param path
 */
void freePath(List* path)
{
	ListElement* current = path->head;
	ListElement* tmp;
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
double heuristicCost(Node* a, Node* b)
{
	double dx = abs(a->x - b->x);
	double dy = abs(a->y - b->y);
	double d_min = fmin(dx, dy);
	double d_max = fmax(dx, dy);
	double diagonal_cost = sqrt(2);
	double orthogonal_cost = 1;

	return diagonal_cost * d_min + orthogonal_cost * (d_max - d_min);
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

	return positionA->distance - positionB->distance;
}

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
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end)
{
	InfoLine line;
	Pos2Dint point;

	initLine(start.x, start.y, end.x, end.y, &line);

	/* Parcourir les points de la ligne */
	while (nextPoint(&line, &point, +1) > 0) {
		if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height) {
			/* Point en dehors des limites de la carte */
			return 0;
		}

		if (map[point.y][point.x] == '.') {
			/* Mur détecté */
			return 0;
		}
	}

	/* Aucun mur détecté, le chemin est dégagé */
	return 1;
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
void findEndPositions(char** map, int width, int height, Node* start, Node** end, int secondX, int secondY, int thirdX, int thirdY, int speedX,
					  int speedY)
{
	int x, y;
	int i;
	EndPosition* endPositions;
	double distance;
	EndPosition endPosition;

	int endPositionCount = 0;
	endPositions = (EndPosition*)malloc(sizeof(EndPosition) * width * height);

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
				endPositions[endPositionCount++] = endPosition;
			}
		}
	}

	qsort(endPositions, endPositionCount, sizeof(EndPosition), compareEndPositions);

	for (i = 0; i < endPositionCount; i++) {
		int x = endPositions[i].x;
		int y = endPositions[i].y;
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
void determineAcceleration(List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY)
{
	int nextX, nextY;
	Node* first;
	if (path == NULL || path->head == NULL || path->head->data == NULL) {
		fprintf(stderr, "Path is NULL\n");
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
	return (int)(speedX * speedX + speedY * speedY);
}

int shouldExploreNeighbor(Node* currentNode, char** map, int width, int height, int newX, int newY, int newSpeedX, int newSpeedY, Pos2Dint currentPos,
						  Pos2Dint newPos, int secondX, int secondY, int thirdX, int thirdY, int maxGas, int accX, int accY)
{
	int newGas;
	int gasCost;

	if (newX == currentNode->x && newY == currentNode->y) {
		return 0; /* ignorer le noeud lui-même */
	}

	if (newX >= width || newY >= height || newX < 0 || newY < 0) {
		return 0;
	}

	if (map[newY][newX] == '.') {
		return 0;
	}

	if (map[newY][newX] == '~' && (newSpeedX * newSpeedX + newSpeedY * newSpeedY >= 1)) {
		return 0;
	}

	if (isPathClear(map, width, height, currentPos, newPos) == 0) {
		return 0;
	}

	if (isPositionOccupied(newX, newY, secondX, secondY, thirdX, thirdY) == 1) {
		return 0;
	}

	if (SpeedNorme(newSpeedX, newSpeedY) > 25) {
		return 0;
	}

	gasCost = gasConsumption(accX, accY, currentNode->speedX, currentNode->speedY, 0);
	newGas = currentNode->gas + gasCost;
	if (newGas < 0 || newGas > maxGas) {
		return 0;
	}

	return 1;
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
 * @param maxGas
 * @return List* le chemin le plus court
 */
List* aStar(Node* start, Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY, int maxGas,
			int currentSpeedX, int currentSpeedY)
{
	int accX;
	int accY;
	int newSpeedX;
	int newSpeedY;
	int newX;
	int newY;
	int gasCost;
	int newGas;
	double distance;
	int penalty = 0;
	Node* neighbour;
	Pos2Dint currentPos;
	Pos2Dint newPos;

	List* openSet = initList();
	List* closedSet = initList();

	start->g_cost = 0;
	start->h_cost = heuristicCost(start, end);
	start->f_cost = start->g_cost + start->h_cost;
	start->gas = maxGas;
	start->speedX = currentSpeedX;
	start->speedY = currentSpeedY;

	addNodeToList(start, openSet);

	while (!isListEmpty(openSet)) {
		Node* currentNode = removeNodeWithLowestFCost(openSet);

		if (nodeEqualsWithoutSpeed(currentNode, end) == 1) {
			List* path = initList();
			Node* pathNode = currentNode;
			while (pathNode != NULL) {
				addNodeToList(pathNode, path);
				pathNode = pathNode->parent;
			}
			return path;
		}

		addNodeToList(currentNode, closedSet);

		/* Générer les voisins */
		for (accX = -1; accX <= 1; accX++) {
			for (accY = -1; accY <= 1; accY++) {
				penalty = 0;
				newSpeedX = currentNode->speedX + accX;
				newSpeedY = currentNode->speedY + accY;

				newX = currentNode->x + newSpeedX;
				newY = currentNode->y + newSpeedY;

				currentPos.x = currentNode->x;
				currentPos.y = currentNode->y;
				newPos.x = newX;
				newPos.y = newY;

				if (shouldExploreNeighbor(currentNode, map, width, height, newX, newY, newSpeedX, newSpeedY, currentPos, newPos, secondX, secondY,
										  thirdX, thirdY, maxGas, accX, accY) == 0) {
					continue;
				}

				gasCost = gasConsumption(accX, accY, currentNode->speedX, currentNode->speedY, 0);
				newGas = currentNode->gas + gasCost;

				neighbour = createNode(newX, newY, currentNode, newSpeedX, newSpeedY, newGas);
				distance = sqrt((newX - currentNode->x) * (newX - currentNode->x) + (newY - currentNode->y) * (newY - currentNode->y));

				if (currentNode->parent != NULL) {
					int previousSpeedX = currentNode->parent->speedX;
					int previousSpeedY = currentNode->parent->speedY;

					if (previousSpeedX != newSpeedX || previousSpeedY != newSpeedY) {
						penalty = 50;
					}
				}

				neighbour->g_cost = currentNode->g_cost + distance + penalty;

				if (map[newY][newX] == '~') {
					neighbour->g_cost = currentNode->g_cost + 4;
				}
				neighbour->h_cost = heuristicCost(neighbour, end);

				if (neighbour->gas < neighbour->h_cost) {
					free(neighbour);
					continue;
				}

				neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;

				if (!nodeInList(neighbour, closedSet)) {
					ListElement* existingElementInOpenSet;
					Node* existingNodeInOpenSet = findNodeInList(neighbour, openSet, &existingElementInOpenSet);

					if (existingNodeInOpenSet == NULL || neighbour->g_cost < existingNodeInOpenSet->g_cost) {
						if (existingNodeInOpenSet != NULL) {
							removeElementFromList(openSet, existingElementInOpenSet);
						}
						addNodeToList(neighbour, openSet);
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
	int width, height;
	int gasLevel;
	int boosts = BOOSTS_AT_START;
	int round = 0;
	int accelerationX = 1, accelerationY = 0;
	int speedX = 0, speedY = 0;
	char action[100];
	char line_buffer[MAX_LINE_LENGTH];
	char** map;
	int myX, myY, secondX, secondY, thirdX, thirdY;

	Node* start = NULL;
	Node* end = NULL;
	List* path = NULL;

	boosts = boosts;							/* Prevent warning "unused variable" */
	fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
	sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
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
		round++;
		fprintf(stderr, "=== ROUND %d\n", round);
		fflush(stderr);
		fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots */
		sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
		fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n", myX, myY, secondX, secondY, thirdX, thirdY);
		fflush(stderr);

		/* Trouver les positions de départ et d'arrivée sur la carte */
		start = createNode(myX, myY, NULL, speedX, speedY, 0);
		findEndPositions(map, width, height, start, &end, secondX, secondY, thirdX, thirdY, speedX, speedY);
		fprintf(stderr, "    Start: (%d, %d)\n", start->x, start->y);
		fprintf(stderr, "    End: (%d, %d)\n", end->x, end->y);
		fflush(stderr);

		/* Executer l'algorithme A* pour trouver le chemin */
		path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY);
		fprintf(stderr, "    Path found: \n");
		reverseList(path);
		printPath(path);

		/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
		determineAcceleration(path, myX, myY, &accelerationX, &accelerationY, speedX, speedY);
		fprintf(stderr, "    Acceleration: (%d, %d)\n", accelerationX, accelerationY);

		/* Gas consumption cannot be accurate here. */
		gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);
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
