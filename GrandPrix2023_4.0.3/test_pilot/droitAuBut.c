#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5
#define INFINITE 9999999

/* Structures utiles au projet */

typedef struct Node {
	int x;
	int y;
	int g_cost;
	int h_cost;
	int f_cost;
	struct Node* parent;
} Node;

typedef struct ListElement {
	void* data;
	struct ListElement* next;
} ListElement;

typedef struct List {
	ListElement* head;
} List;

typedef struct {
	int x;
	int y;
	int distance;
} EndPosition;

/* Fonctions utiles pour la gestion des noeuds, listes, coûts... */
Node* createNode(int x, int y, Node* parent)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->x = x;
	newNode->y = y;
	newNode->parent = parent;
	newNode->g_cost = 0;
	newNode->h_cost = 0;
	newNode->f_cost = 0;
	return newNode;
}

int heuristicCost(Node* a, Node* b)
{
	return abs(a->x - b->x) + abs(a->y - b->y);
}

double calculateAngle(int dx, int dy)
{
	return atan2(dy, dx);
}

int nodeInList(Node* node, List* list)
{
	ListElement* current = list->head;
	while (current != NULL) {
		Node* current_node = (Node*)current->data;
		if (current_node->x == node->x && current_node->y == node->y) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

Node* findNodeInList(Node* node, List* list, ListElement** elementInList)
{
	ListElement* currentElement = list->head;

	while (currentElement != NULL) {
		Node* currentNode = (Node*)currentElement->data;

		if (currentNode->x == node->x && currentNode->y == node->y) {
			if (elementInList != NULL) {
				*elementInList = currentElement;
			}
			return currentNode;
		}

		currentElement = currentElement->next;
	}

	return NULL;
}

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

List* initList()
{
	List* newList = (List*)malloc(sizeof(List));
	newList->head = NULL;
	return newList;
}

int isListEmpty(List* list)
{
	return list->head == NULL;
}

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

void reverseList(List* list)
{
	ListElement* prevElement = NULL;
	ListElement* currentElement = list->head;
	ListElement* nextElement = NULL;

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

int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY)
{
	return (x == secondX && y == secondY) || (x == thirdX && y == thirdY);
}

int compareEndPositions(const void* a, const void* b)
{
	const EndPosition* positionA = (const EndPosition*)a;
	const EndPosition* positionB = (const EndPosition*)b;

	return positionA->distance - positionB->distance;
}

int isWallBetweenNodes(Node* node1, Node* node2, char** map, int width, int height)
{
	int dx = abs(node2->x - node1->x);
	int dy = abs(node2->y - node1->y);
	int x = node1->x;
	int y = node1->y;
	int n = 1 + dx + dy;
	int x_inc = (node2->x > node1->x) ? 1 : -1;
	int y_inc = (node2->y > node1->y) ? 1 : -1;
	int error = dx - dy;
	dx *= 2;
	dy *= 2;

	for (; n > 0; --n) {
		if (x >= 0 && x < width && y >= 0 && y < height && (map[y][x] == '.')) {
			return 1; /* There's a wall between the nodes. */
		}

		if (error > 0) {
			x += x_inc;
			error -= dy;
		} else {
			y += y_inc;
			error += dx;
		}
	}

	return 0; /* No wall between the nodes. */
}

/* A star */
List* aStar(Node* start, Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY)
{
	int accX;
	int accY;
	int speedX;
	int speedY;

	List* openSet = initList();
	List* closedSet = initList();

	start->g_cost = 0;
	start->h_cost = heuristicCost(start, end);
	start->f_cost = start->g_cost + start->h_cost;

	addNodeToList(start, openSet);

	while (!isListEmpty(openSet)) {
		Node* currentNode = removeNodeWithLowestFCost(openSet);

		if (currentNode->x == end->x && currentNode->y == end->y) {
			/* Chemin trouvé, reconstruire le chemin et le retourner */
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
		for (speedX = -4; speedX <= 4; speedX++) {
			for (speedY = -4; speedY <= 4; speedY++) {
				for (accX = -1; accX <= 1; accX++) {
					for (accY = -1; accY <= 1; accY++) {
						/* Vérifiez que la norme de la vitesse ne dépasse pas 4 */
						if ((speedX + accX) * (speedX + accX) + (speedY + accY) * (speedY + accY) > 25) {
							continue;
						}

						int newX = currentNode->x + speedX + accX;
						int newY = currentNode->y + speedY + accY;
						if (newX == currentNode->x && newY == currentNode->y) {
							continue; /* ignorer le noeud lui-même */
						}

						/* if ((newX >= 0 && newX < width && newY >= 0) && (map[newY][newX] == '~') &&
							((speedX + accX) * (speedX + accX) + (speedY + accY) * (speedY + accY) > 2)) {
							continue;
						} */

						/* Vérifier si les coordonnées sont valides et si le terrain est praticable */
						if (newX >= 0 && newX < width && newY >= 0 && newY < height &&
							(map[newY][newX] == '#' || map[newY][newX] == '=' || map[newY][newX] == '~') &&
							(isPositionOccupied(newX, newY, secondX, secondY, thirdX, thirdY) == 0) &&
							(isWallBetweenNodes(currentNode, createNode(newX, newY, NULL), map, width, height) == 0)) {
							Node* neighbour = createNode(newX, newY, currentNode);
							neighbour->g_cost = currentNode->g_cost + 1;

							/* Ajoutez une pénalité pour les virages rapides */
							if (currentNode->parent != NULL) { /* Vérifiez si le parent existe */
								double angle1 = calculateAngle(currentNode->x - currentNode->parent->x, currentNode->y - currentNode->parent->y);
								double angle2 = calculateAngle(newX - currentNode->x, newY - currentNode->y);
								double angleDifference = fabs(angle2 - angle1);

								/* Si la différence d'angle est supérieure à un seuil (par exemple, PI/4 radians), ajoutez une pénalité */
								if (angleDifference > 3.1415 / 4) {
									neighbour->g_cost += 15 * angleDifference; /* Vous pouvez ajuster le coefficient pour modifier l'importance de la
																			  pénalité */
								}
							}
							if (map[newY][newX] == '~') {
								neighbour->g_cost = currentNode->g_cost + 4;
							}
							neighbour->h_cost = heuristicCost(neighbour, end);
							neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;

							if (!nodeInList(neighbour, closedSet)) {
								ListElement* existingElementInOpenSet;
								/* Vérifie si le voisin est déjà dans l'ensemble ouvert et s'il y'a un meilleur chemin */
								Node* existingNodeInOpenSet = findNodeInList(neighbour, openSet, &existingElementInOpenSet);
								if (existingNodeInOpenSet == NULL || neighbour->g_cost < existingNodeInOpenSet->g_cost) {
									if (existingNodeInOpenSet != NULL) {
										if (existingElementInOpenSet == openSet->head) {
											openSet->head = existingElementInOpenSet->next;
										} else {
											ListElement* previous = openSet->head;
											while (previous->next != existingElementInOpenSet) {
												previous = previous->next;
											}
											previous->next = existingElementInOpenSet->next;
										}
										free(existingElementInOpenSet);
									}
									addNodeToList(neighbour, openSet);
								}
							}
						}
					}
				}
			}
		}
	}
	/* Pas de chemin trouvé */
	return NULL;
}

/* Trouver les positions de départ et d'arrivée sur la carte */
void findEndPositions(char** map, int width, int height, Node* start, Node** end, int secondX, int secondY, int thirdX, int thirdY)
{
	int x, y;
	int i;
	EndPosition* endPositions;
	int distance;
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
			*end = createNode(x, y, NULL);
			break;
		}
	}
}

/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
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

	fprintf(stderr, "Next node in path: (%d, %d)\n", nextX, nextY);
	fprintf(stderr, "Current position: (%d, %d)\n", myX, myY);
	fprintf(stderr, "Current speed: (%d, %d)\n", speedX, speedY);

	/* Vérifier si la vitesse actuelle est suffisante pour atteindre la case suivante */
	if (myX + speedX == nextX && myY + speedY == nextY) {
		*accelerationX = 0;
		*accelerationY = 0;
	} else {
		*accelerationX = nextX - myX - speedX;
		*accelerationY = nextY - myY - speedY;
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

	fprintf(stderr, "First node in path: (%d, %d)\n", first->x, first->y);
	fprintf(stderr, "Current position: (%d, %d)\n", myX, myY);
	fprintf(stderr, "Acceleration: (%d, %d)\n", *accelerationX, *accelerationY);
}

/* Libérer la mémoire utilisée par la liste */
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

void freeNode(Node* node)
{
	if (node != NULL) {
		free(node);
	}
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
		int myX, myY, secondX, secondY, thirdX, thirdY;
		round++;
		fprintf(stderr, "=== ROUND %d\n", round);
		fflush(stderr);
		fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots */
		sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
		fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n", myX, myY, secondX, secondY, thirdX, thirdY);
		fflush(stderr);

		/* Trouver les positions de départ et d'arrivée sur la carte */
		start = createNode(myX, myY, NULL);
		findEndPositions(map, width, height, start, &end, secondX, secondY, thirdX, thirdY);
		fprintf(stderr, "    Start: (%d, %d)\n", start->x, start->y);
		fprintf(stderr, "    End: (%d, %d)\n", end->x, end->y);
		fflush(stderr);

		/* Executer l'algorithme A* pour trouver le chemin */
		path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY);
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
