/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/priorityQueue.h"
#include "../include/list.h"
#include "../include/hashSet.h"
#include "../include/aStar.h"
#include "../include/follow_line.h"

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

int main(void)
{
	int row;
	int width;
	int height;
	int gasLevel;
	int maxGas;
	int boosts;
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
	int oldX = 0;
	int oldY = 0;
	int occupied;

	ArrayEnd* arrayEnd = NULL;
	Node* start = NULL;
	Node* end = NULL;
	List* path = NULL;
	occupied = 0;

	if (round == 0) {
		boosts = BOOSTS_AT_START;
	}

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
			boosts = BOOSTS_AT_START;
		}

		start->x = myX;
		start->y = myY;
		if (round != 1){
			start->gas = gasLevel;
		}
		start->boostRemaining = boosts;
		fprintf(stderr, "Boost Remaining : %d", start->boostRemaining);

		findBestEnd(myX, myY, secondX, secondY, thirdX, thirdY, speedX, speedY, arrayEnd, &end);
		/* Executer l'algorithme A* pour trouver le chemin */
		path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
		while (path == NULL && vitesse > 0) {
			vitesse--;
			path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
		}
		reverseList(path);

		while (path == NULL && i < arrayEnd->size) {
			vitesse = 25;
			end = createNode(arrayEnd->array[i].x, arrayEnd->array[i].y, NULL, speedX, speedY, 0);
			path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
			while (path == NULL && vitesse > 0) {
				vitesse--;
				path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
			}
			reverseList(path);
			i++;
		}

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
				path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
				while (path == NULL && vitesse > 0) {
					vitesse--;
					path = aStar(start, end, map, width, height, secondX, secondY, thirdX, thirdY, gasLevel, speedX, speedY, vitesse, occupied);
				}
				reverseList(path);
			}
		}

		/* Utiliser le chemin trouvé par A* pour déterminer l'accélération */
		determineAcceleration(path, myX, myY, &accelerationX, &accelerationY, speedX, speedY, map);
		if (abs(accelerationX) > 1 || abs(accelerationY) > 1) {
			fprintf(stderr, "\nBoost utilisé !\n");
			boosts--;
		}

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
