#ifndef ASTAR_H
#define ASTAR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"

List* getPath(Node* currentNode);
Node* createNeighbourNode(int newX, int newY, Node* currentNode, int newSpeedX, int newSpeedY, int newGas, char** map, const Node* end);
int SpeedNorme(int speedX, int speedY);
double heuristicCost(const Node* a, const Node* b);
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);
int isPositionOccupied(int x, int y, int secondX, int secondY, int thirdX, int thirdY);
int isPathClear(char** map, int width, int height, Pos2Dint start, Pos2Dint end);
int isPathClear_Occupied(char** map, int width, int height, Pos2Dint start, Pos2Dint end, int secondX, int secondY, int thirdX, int thirdY);
ArrayEnd* findEndPositions(char** map, int width, int height, const Node* start);
int compareEndPositions(const void* a, const void* b);
void findBestEnd(int myX, int myY, int secondX, int secondY, int thirdX, int thirdY, int speedX, int speedY, ArrayEnd* array, Node** end);
void determineAcceleration(const List* path, int myX, int myY, int* accelerationX, int* accelerationY, int speedX, int speedY, char** map);
int shouldContinue(int newX, int newY, int width, int height, char** map, int currentNodeX, int currentNodeY, int accX, int accY);
List* aStar(Node* start, const Node* end, char** map, int width, int height, int secondX, int secondY, int thirdX, int thirdY, int gasLevel,
			int currentSpeedX, int currentSpeedY, int speedMax, int occupied);

#endif /* ASTAR_H */
