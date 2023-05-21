#ifndef ASTAR_H
#define ASTAR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

/**
 * @brief Get the Path from the end node to the start node
 *
 * @param currentNode The end node
 * @return List* The path from the end node to the start node
 */
List* getPath(Node* currentNode);

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
Node* createNeighbourNode(int newX, int newY, Node* currentNode, int newSpeedX, int newSpeedY, int newGas, char** map, const Node* end);

/**
 * @brief Get the speed norme from the speedX and speedY
 *
 * @param speedX
 * @param speedY
 * @return int The speed norme
 */
int SpeedNorme(int speedX, int speedY);

/**
 * @brief Get the heuristic cost from two nodes
 *
 * @param a
 * @param b
 * @return double The heuristic cost
 */
double heuristicCost(const Node* a, const Node* b);

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
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

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
int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY);

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
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end);

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
int isPathClear_Occupied(char** map, int width, int height, Pos2Dint start, Pos2Dint end, int secondX, int secondY, int thirdX, int thirdY);

/**
 * @brief Find all the end positions
 *
 * @param map
 * @param width
 * @param height
 * @param start
 * @return ArrayEnd*  The array of all the end positions
 */
ArrayEnd* findEndPositions(char** map, int width, int height, const Node* start);

/**
 * @brief Compare two end positions
 *
 * @param a
 * @param b
 * @return int -1 if a < b, 0 if a == b, 1 if a > b
 */
int compareEndPositions(const void* a, const void* b);

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
void findBestEnd(int myX, int myY, int secondX, int secondY, int thirdX, int thirdY, int speedX, int speedY, ArrayEnd* array, Node** end);

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
void determineAcceleration(const List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY, char** map);

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
int shouldContinue(int newX, int newY, int width, int height, char** map, int currentNodeX, int currentNodeY, int accX, int accY);

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
			int currentSpeedX, int currentSpeedY, int speedMax, int occupied);

#endif /* ASTAR_H */
