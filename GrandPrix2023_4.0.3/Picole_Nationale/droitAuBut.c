#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

typedef struct {
	int x;
	int y;
} Pos2Dint;

typedef struct {
	float x;
	float y;
} Pos2Dfloat;

typedef struct {
	Pos2Dint start;
	Pos2Dint end;
	Pos2Dfloat currentPosition;
	Pos2Dfloat delta; /*< Shift vector */
	int len;		  /*< Length along the main axis */
	int pos;		  /*< Reference position on the line */
} InfoLine;

void initLine(int x1, int y1, int x2, int y2, InfoLine* infoLine)
{
	int adxi, adyi, dxi, dyi;
	infoLine->start.x = x1;
	infoLine->start.y = y1;
	infoLine->currentPosition.x = x1 + 0.5;
	infoLine->currentPosition.y = y1 + 0.5;
	infoLine->end.x = x2;
	infoLine->end.y = y2;

	adxi = dxi = x2 - x1;
	adyi = dyi = y2 - y1;
	if (adxi < 0) {
		adxi = -dxi;
	}
	if (adyi < 0) {
		adyi = -dyi;
	}
	infoLine->pos = 0;
	infoLine->len = adxi;
	if (adyi > adxi) {
		infoLine->len = adyi;
	}
	infoLine->delta.x = ((float)dxi) / infoLine->len;
	infoLine->delta.y = ((float)dyi) / infoLine->len;
}

int nextPoint(InfoLine* infoLine, Pos2Dint* point, int direction)
{
	if (direction > 0) {
		if (infoLine->pos == infoLine->len) {
			point->x = infoLine->end.x;
			point->y = infoLine->end.y;
			return -1; /* End of the line */
		}
		infoLine->currentPosition.x += infoLine->delta.x;
		infoLine->currentPosition.y += infoLine->delta.y;
		point->x = ((int)infoLine->currentPosition.x);
		point->y = ((int)infoLine->currentPosition.y);
		infoLine->pos++;
		return 1; /* a new point is found */
	}
	if (direction < 0) {
		if (infoLine->pos == 0) {
			point->x = infoLine->start.x;
			point->y = infoLine->start.y;
			return -1; /* End of the line */
		}
		infoLine->currentPosition.x -= infoLine->delta.x;
		infoLine->currentPosition.y -= infoLine->delta.y;
		point->x = ((int)infoLine->currentPosition.x);
		point->y = ((int)infoLine->currentPosition.y);
		infoLine->pos--;

		return 1; /* a new point is found */
	}
	return 1;	  /* direction == 0 => no motion */
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

Pos2Dint calculate_optimal_acceleration(int myX, int myY, int speedX, int speedY, int gasLevel, int width, int height, char** map)
{
	int accX;
	int accY;
	Pos2Dint optimalAcceleration = { 0, 0 };
	int minGasCost = 1000000;
	for (accX = -1; accX <= 1; accX++) {
		for (accY = -1; accY <= 1; accY++) {
			int newX = myX + speedX + accX;
			int newY = myY + speedY + accY;
			if (newX >= 0 && newX < width && newY >= 0 && newY < height && map[newY][newX] != '#') {
				int gasCost = gasConsumption(accX, accY, speedX, speedY, map[newY][newX] == '~');
				if (gasCost < minGasCost && gasLevel + gasCost > 0) {
					minGasCost = gasCost;
					optimalAcceleration.x = accX;
					optimalAcceleration.y = accY;
				}
			}
		}
	}

	return optimalAcceleration;
}

int detect_collisions(int myX, int myY, int speedX, int speedY, int width, int height, char** map)
{
	Pos2Dint start;
	Pos2Dint end;
	InfoLine lineInfo;
	start.x = myX;
	start.y = myY;
	end.x = myX + speedX;
	end.y = myY + speedY;

	if (end.x < 0 || end.x >= width || end.y < 0 || end.y >= height) {
		return 1;
	}

	initLine(start.x, start.y, end.x, end.y, &lineInfo);

	Pos2Dint currentPosition;
	while (nextPoint(&lineInfo, &currentPosition, +1) > 0) {
		char terrain = map[currentPosition.y][currentPosition.x];

		if (terrain == '#') {
			return 1;
		}
	}

	return 0;
}

void avoid_obstacles(int* accelerationX, int* accelerationY, int speedX, int speedY, int myX, int myY, int width, int height, char** map)
{
	int accX;
	int accY;
	int bestAccX = 0, bestAccY = 0;
	int minCollisionDistance = -1;

	for (accX = -1; accX <= 1; ++accX) {
		for (accY = -1; accY <= 1; ++accY) {
			int newX = myX + speedX + accX;
			int newY = myY + speedY + accY;

			if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
				int collisionDistance = 0;

				while (!detect_collisions(myX, myY, speedX + accX, speedY + accY, width, height, map)) {
					myX += speedX + accX;
					myY += speedY + accY;
					collisionDistance++;
				}

				if (collisionDistance > minCollisionDistance) {
					minCollisionDistance = collisionDistance;
					bestAccX = accX;
					bestAccY = accY;
				}
			}
		}
	}

	*accelerationX = bestAccX;
	*accelerationY = bestAccY;
}

int find_shortest_path(int startX, int startY, int endX, int endY, int width, int height, char** map)
{
	int distance[height][width];
	int visited[height][width];
	int i, j;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			distance[i][j] = INFINITY;
			visited[i][j] = 0;
		}
	}

	distance[startY][startX] = 0;

	while (1) {
		int min_distance = INFINITY;
		int min_x = -1;
		int min_y = -1;

		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (!visited[i][j] && distance[i][j] < min_distance) {
					min_distance = distance[i][j];
					min_x = j;
					min_y = i;
				}
			}
		}

		if (min_x == -1 && min_y == -1) {
			break;
		}

		if (min_x == endX && min_y == endY) {
			break;
		}

		visited[min_y][min_x] = 1;

		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {
				int new_x = min_x + j;
				int new_y = min_y + i;

				if (new_x >= 0 && new_y >= 0 && new_x < width && new_y < height && map[new_y][new_x] != '#' && !visited[new_y][new_x]) {
					int new_distance = distance[min_y][min_x] + 1;
					if (new_distance < distance[new_y][new_x]) {
						distance[new_y][new_x] = new_distance;
					}
				}
			}
		}
	}

	return distance[endY][endX];
}

int apply_boost(int* boosts, int* accelerationX, int* accelerationY, int speedX, int speedY, int myX, int myY, int width, int height, char** map)
{
	int i;
	if (*boosts <= 0) {
		return 0;
	}

	int straight_line_length = 0;
	for (i = 1; i <= 5; ++i) {
		int newX = myX + i * (*accelerationX);
		int newY = myY + i * (*accelerationY);
		if (newX >= 0 && newX < width && newY >= 0 && newY < height && map[newY][newX] != '#') {
			straight_line_length++;
		} else {
			break;
		}
	}

	if (straight_line_length >= 4) {
		*boosts -= 1;
		*accelerationX *= 2;
		*accelerationY *= 2;
		return 1;
	}

	return 0;
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
	char** grid;
	Pos2Dint accel;
	int collision;
	int pathDistance;

	boosts = boosts;							/* Prevent warning "unused variable" */
	fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
	sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
	fprintf(stderr, "=== >Map< ===\n");
	fprintf(stderr, "Size %d x %d\n", width, height);
	fprintf(stderr, "Gas at start %d \n\n", gasLevel);
	grid = (char**)malloc(height * sizeof(char*));
	for (row = 0; row < height; ++row) { /* Read map data, line per line */
		fgets(line_buffer, MAX_LINE_LENGTH, stdin);
		fputs(line_buffer, stderr);
		grid[row] = malloc(width * sizeof(char));
		strcpy(grid[row], line_buffer);
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

		/* Calculate optimal acceleration */
		accel = calculate_optimal_acceleration(myX, myY, speedX, speedY, gasLevel, width, height, grid);
		accelerationX = accel.x;
		accelerationY = accel.y;
		fprintf(stderr, "    Acceleration: (%d,%d)\n", accelerationX, accelerationY);
		fflush(stderr);

		/* Detection collisions */
		/* collision = detect_collisions(myX, myY, speedX, speedY, width, height, grid);
		if (collision == 0) {
			fprintf(stderr, " Collision !!!\n");
		}
		fflush(stderr); */

		/* Avoid obstacles */
		/* avoid_obstacles(&accelerationX, &accelerationY, speedX, speedY, myX, myY, width, height, grid);
		fprintf(stderr, "    Acceleration after obstacle avoidance: (%d,%d)\n", accelerationX, accelerationY);
		fflush(stderr); */

		/* Find shortest path */
		pathDistance = find_shortest_path(myX, myY, secondX, secondY, width, height, grid);
		fprintf(stderr, "    Distance to second: %d\n", pathDistance);
		fflush(stderr);

		/* Apply boost */
		apply_boost(&boosts, &accelerationX, &accelerationY, speedX, speedY, myX, myY, width, height, grid);

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

	return EXIT_SUCCESS;
}
