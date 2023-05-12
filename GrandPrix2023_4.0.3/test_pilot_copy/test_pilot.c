#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

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

int nodeInList(const Node *node, List* list)
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

	Node* result = (Node*)lowest->data;
	free(lowest);
	return result;
}

List* initList()
{
	List* newList = (List*)malloc(sizeof(List));
	newList->head = NULL;
	return newList;
}

int isListEmpty(const List *list)
{
	return list->head == NULL;
}

/* A star */
List* aStar(Node* start, Node* end, char** map, int width, int height)
{
	int dx;
	int dy;
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
		for (dx = -1; dx <= 1; dx++) {
			for (dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0) {
					continue; /* ignorer le noeud lui-même */
				}

				int newX = currentNode->x + dx;
				int newY = currentNode->y + dy;

				/* Vérifier si les coordonnées sont valides et si le terrain est praticable */
				if (newX >= 0 && newX < width && newY >= 0 && newY < height && (map[newY][newX] == '#' || map[newY][newX] == '=')) {
					Node* neighbour = createNode(newX, newY, currentNode);
					neighbour->g_cost = currentNode->g_cost + 1;
					neighbour->h_cost = heuristicCost(neighbour, end);
					neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;

					if (nodeInList(neighbour, closedSet) && !nodeInList(neighbour, openSet)) {
						addNodeToList(neighbour, openSet);
					}
				}
			}
		}
	}

	/* Pas de chemin trouvé */
	return NULL;
}

/* Trouver les positions de départ et d'arrivée sur la carte */
void findStartAndEndPositions(char** map, int width, int height, Node** start, Node** end)
{
	int x;
	int y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (map[y][x] == '1' || map[y][x] == '2' || map[y][x] == '3') {
				*start = createNode(x, y, NULL);
			} else if (map[y][x] == '=') {
				*end = createNode(x, y, NULL);
			}
		}
	}
}

/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
void determineAcceleration(List* path, int myX, int myY, int* accelerationX, int* accelerationY)
{
	if (path == NULL || path->head == NULL || path->head->data == NULL) {
		*accelerationX = 0;
		*accelerationY = 0;
		return;
	}

	Node* first = path->head->next->data;
	*accelerationX = first->x - myX;
	*accelerationY = first->y - myY;
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

	map = (char**)malloc(height * sizeof(char*));
	boosts = boosts;							/* Prevent warning "unused variable" */
	fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
	sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
	fprintf(stderr, "=== >Map< ===\n");
	fprintf(stderr, "Size %d x %d\n", width, height);
	fprintf(stderr, "Gas at start %d \n\n", gasLevel);
	for (row = 0; row < height; ++row) { /* Read map data, line per line */
		fgets(line_buffer, MAX_LINE_LENGTH, stdin);
		fputs(line_buffer, stderr);
		map[row] = (char*)malloc(width * sizeof(char));
		strcpy(map[row], line_buffer);
		map[row][width] = '\0'; /* Remove trailing \n */
	}
	fflush(stderr);
	fprintf(stderr, "\n=== Race start ===\n");

	/* Trouver les positions de départ et d'arrivée sur la carte */
	findStartAndEndPositions(map, width, height, &start, &end);

	/* Executer l'algorithme A* pour trouver le chemin */
	path = aStar(start, end, map, width, height);
	while (!feof(stdin)) {
		int myX, myY, secondX, secondY, thirdX, thirdY;
		round++;
		fprintf(stderr, "=== ROUND %d\n", round);
		fflush(stderr);
		fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots */
		sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
		fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n", myX, myY, secondX, secondY, thirdX, thirdY);
		fflush(stderr);

		/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
		determineAcceleration(path, myX, myY, &accelerationX, &accelerationY);

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
