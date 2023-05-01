#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

#include <limits.h>
#include "follow_line.h"
#define INF INT_MAX

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	int cost;
	Position pos;
} Node;

int is_valid_move(char** map, int x, int y, int width, int height)
{
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return 0;
	}
	return map[y][x] != '.';
}

int get_cost(char terrain)
{
	if (terrain == '#') {
		return 1;
	} else if (terrain == '~') {
		return 5;
	} else {
		return INF;
	}
}

Position dijkstra_next_move(char** map, int width, int height, Position start, Position end)
{
	int** visited;
	int** dist;
	int y;
	int x;
	int i;
	Position** prev;
	Position directions[] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }, { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };
	int dir_count;

	visited = malloc(height * sizeof(int*));
	if (visited == NULL) {
		perror("Memory allocation failed");
	}
	for (i = 0; i < height; i++) {
		visited[i] = malloc(width * sizeof(int));
		if (visited[i] == NULL) {
			perror("Memory allocation failed");
		}
	}

	dist = malloc(height * sizeof(int*));
	if (dist == NULL) {
		perror("Memory allocation failed");
	}
	for (i = 0; i < height; i++) {
		dist[i] = malloc(width * sizeof(int));
		if (dist[i] == NULL) {
			perror("Memory allocation failed");
		}
	}
	prev = malloc(height * sizeof(Position*));
	if (prev == NULL) {
		perror("Memory allocation failed");
	}
	for (i = 0; i < height; i++) {
		prev[i] = malloc(width * sizeof(Position));
		if (prev[i] == NULL) {
			perror("Memory allocation failed");
		}
	}

	dir_count = sizeof(directions) / sizeof(directions[0]);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			visited[y][x] = 0;
			dist[y][x] = INF;
		}
	}

	dist[start.y][start.x] = 0;

	while (1) {
		Node min_node;
		min_node.cost = INF;

		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				if (!visited[y][x] && dist[y][x] < min_node.cost) {
					min_node.cost = dist[y][x];
					min_node.pos.x = x;
					min_node.pos.y = y;
				}
			}
		}

		if (min_node.cost == INF || (min_node.pos.x == end.x && min_node.pos.y == end.y)) {
			break;
		}

		x = min_node.pos.x;
		y = min_node.pos.y;
		visited[y][x] = 1;

		for (i = 0; i < dir_count; i++) {
			int dx = directions[i].x;
			int dy = directions[i].y;
			int nx = x + dx;
			int ny = y + dy;

			if (is_valid_move(map, nx, ny, width, height) && !visited[ny][nx]) {
				int new_cost = dist[y][x] + get_cost(map[ny][nx]);
				if (new_cost < dist[ny][nx]) {
					dist[ny][nx] = new_cost;
					prev[ny][nx].x = x;
					prev[ny][nx].y = y;
				}
			}
		}
	}

	if (dist[end.y][end.x] == INF) {
		Position pos;
		pos.x = -1;
		pos.y = -1;
		return pos;
	} else {
		Position current = end;
		while (!(prev[current.y][current.x].x == start.x && prev[current.y][current.x].y == start.y)) {
			current = prev[current.y][current.x];
		}
		return current;
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
 * @param inSand (0 or 1)
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
	char** grid;
	int x;
	int y;
	int j;
	Position start;
	Position end;
	Position next;
	InfoLine lineInfo;
	Pos2Dint currentPoint;

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
		grid[row] = (char*)malloc((width + 1) * sizeof(char));
		strcpy(grid[row], line_buffer);
		grid[row][width] = '\0';
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
		/* Gas consumption cannot be accurate here. */
		gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);
		speedX += accelerationX;
		speedY += accelerationY;

		/* Calcul de la position d'arrivée */
		start.x = myX;
		start.y = myY;
		for (x = 0; x < width; x++) {
			for (y = 0; y < height; y++) {
				if (grid[y][x] == '=') {
					end.x = x;
					end.y = y;
				}
			}
		}

		next = dijkstra_next_move(grid, width, height, start, end);
		/* accelerationX = next.x - myX;
		accelerationY = next.y - myY; */
		initLine(myX, myY, next.x, next.y, &lineInfo);
		nextPoint(&lineInfo, &currentPoint, 1);
		accelerationX = currentPoint.x - myX;
		accelerationY = currentPoint.y - myY;
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

	for (j = 0; j < height; ++j) {
		free(grid[j]);
	}
	free(grid);

	return EXIT_SUCCESS;
}
