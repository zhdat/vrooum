#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/aStar.h"
#include "../include/priorityQueue.h"
#include "../include/list.h"
#include "../include/hashSet.h"
#include "../include/follow_line.h"

#define fmin(x, y) ((x) < (y) ? (x) : (y))
#define fmax(x, y) ((x) > (y) ? (x) : (y))

/**
 * @brief Get the Path from the end node to the start node
 *
 * @param currentNode The end node
 * @return List* The path from the end node to the start node
 */
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
 * @brief Create a Neighbour Node
 *
 * @param newX
 * @param newY
 * @param currentNode
 * @param newSpeedX
 * @param newSpeedY
 * @param newGas
 * @param map
 * @param end
 * @return Node* The new neighbour node
 */
Node* createNeighbourNode(int newX, int newY, Node* currentNode, int newSpeedX, int newSpeedY, int newGas, char** map, const Node* end)
{
	int factor;
	Node* neighbour = createNode(newX, newY, currentNode, newSpeedX, newSpeedY, newGas);
	factor = 1;
	if (map[newY][newX] == '~') {
		factor += 6;
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

/**
 * @brief Get the speed norme from the speedX and speedY
 *
 * @param speedX
 * @param speedY
 * @return int The speed norme
 */
int SpeedNorme(int speedX, int speedY)
{
	return (speedX * speedX + speedY * speedY);
}

/**
 * @brief Get the heuristic cost from two nodes
 *
 * @param a
 * @param b
 * @return double The heuristic cost
 */
double heuristicCost(const Node* a, const Node* b)
{
	double dx = abs(a->x - b->x);
	double dy = abs(a->y - b->y);
	double dMin = fmin(dx, dy);
	double dMax = fmax(dx, dy);
	double diagonalCost = sqrt(2);
	double orthogonalCost = 1;

	double heuristic1 = dMin * diagonalCost + (dMax - dMin) * orthogonalCost;
	return heuristic1;
}

/**
 * @brief Get the gas cost from two nodes
 *
 * @param accX
 * @param accY
 * @param speedX
 * @param speedY
 * @param inSand
 * @return int The gas cost
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
 * @brief Verify if the position is occupied
 *
 * @param x
 * @param y
 * @param secondX
 * @param secondY
 * @param thirdX
 * @param thirdY
 * @return int 0 if the position is occupied, 1 otherwise
 */
int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY)
{
	return (x == secondX && y == secondY) || (x == thirdX && y == thirdY);
}

/**
 * @brief Verify if between two nodes the path is clear
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @param end
 * @return int 0 if the path is not clear, 1 otherwise
 */
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end)
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

/**
 * @brief Verify if between two nodes the path is clear and if the position is occupied
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
 * @return int 0 if the path is not clear or if the position is occupied, 1 otherwise
 */
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
 * @brief Find all the end positions
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @return ArrayEnd*  The array of all the end positions
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

/**
 * @brief Compare two end positions
 *
 * @param a
 * @param b
 * @return int -1 if a < b, 0 if a == b, 1 if a > b
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
 * @brief Find the best end position
 *
 * @param myX
 * @param myY
 * @param secondX
 * @param secondY
 * @param thirdX
 * @param thirdY
 * @param speedX
 * @param speedY
 * @param array
 * @param end
 */
void findBestEnd(int myX, int myY, int secondX, int secondY, int thirdX, int thirdY, int speedX, int speedY, ArrayEnd* array, Node** end)
{
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
 * @brief Determine the acceleration
 *
 * @param path
 * @param myX
 * @param myY
 * @param accelerationX
 * @param accelerationY
 * @param speedX
 * @param speedY
 * @param map
 */
void determineAcceleration(const List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY, char** map)
{
	int nextX;
	int nextY;
	Node const* first;
	if (path == NULL || path->head == NULL || path->head->data == NULL || path->head->next == NULL || path->head->next->data == NULL) {
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

	/* Accélération dans le sable */
	if (map[myY][myX] == '~') {
		if (SpeedNorme(speedX + *accelerationX, speedY + *accelerationY) > 1) {
			*accelerationX = 0;
			*accelerationY = 0;
		}
	}
}

/**
 * @brief Determine if we need to explore this node
 *
 * @param newX
 * @param newY
 * @param width
 * @param height
 * @param map
 * @param currentNodeX
 * @param currentNodeY
 * @param accX
 * @param accY
 * @return int 0 if we don't need to explore this node, 1 otherwise
 */
int shouldContinue(int newX, int newY, int width, int height, char** map, int currentNodeX, int currentNodeY, int accX, int accY)
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

/**
 * @brief The A* algorithm
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
 * @param gasLevel
 * @param currentSpeedX
 * @param currentSpeedY
 * @param speedMax
 * @param occupied
 * @return List* The path from the start node to the end node
 */
List* aStar(Node* start, const Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY, int gasLevel,
			int currentSpeedX, int currentSpeedY, int speedMax, int occupied)
{
	int accX;
	int accY;
	int newSpeedX;
	int newSpeedY;
	int newX;
	int newY;
	int newGas;
	int acc_boost = 2;
	Node* neighbour;
	Pos2Dint currentPos;
	Pos2Dint newPos;

	PriorityQueue* openSet = pqInit();
	HashSet* closedSet = hsInit();

	start->g_cost = 0;
	start->h_cost = heuristicCost(start, end);
	start->f_cost = start->g_cost + start->h_cost;
	start->gas = gasLevel;
	start->speedX = currentSpeedX;
	start->speedY = currentSpeedY;

	if (start->boostRemaining <= 0 || map[start->y][start->x] == '~') {
		acc_boost = 1;
	}
	if (start->boostRemaining > 0 && map[start->y][start->x] != '~') {
		acc_boost = 2;
	}

	fprintf(stderr, "\nacc_boost : %d\n", acc_boost);
	pqPush(openSet, start);

	while (!pqIsEmpty(openSet)) {
		Node* currentNode = pqPop(openSet);

		if (nodeEqualsWithoutSpeed(currentNode, end) == 1) {
			List* path;
			path = getPath(currentNode);
			return path;
		}

		hsInsert(closedSet, currentNode);

		/* Générer les voisins */
		for (accX = -acc_boost; accX <= acc_boost; accX++) {
			for (accY = -acc_boost; accY <= acc_boost; accY++) {
				newSpeedX = currentNode->speedX + accX;
				newSpeedY = currentNode->speedY + accY;

				if ((newSpeedX * newSpeedX) + (newSpeedY * newSpeedY) <= speedMax) {
					newX = currentNode->x + newSpeedX;
					newY = currentNode->y + newSpeedY;

					if (shouldContinue(newX, newY, width, height, map, currentNode->x, currentNode->y, accX, accY) == 0) {
						continue;
					}

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

					if (isPathClear(map, width, height, currentPos, newPos) == 0)
						continue;

					newGas = currentNode->gas + gasConsumption(accX, accY, newSpeedX, newSpeedY, map[newY][newX] == '~');
					if (newGas < 0)
						continue;

					neighbour = createNeighbourNode(newX, newY, currentNode, newSpeedX, newSpeedY, newGas, map, end);
					neighbour->boostRemaining = currentNode->boostRemaining - ((accX == 2) ? 1 : 0) - ((accY == 2) ? 1 : 0);

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
