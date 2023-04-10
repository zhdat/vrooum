#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Node {
	Point point;
	int g_cost;
	int h_cost;
	int f_cost;
	struct Node* parent;
} Node;

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

int manhattan_distance(Point a, Point b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int points_are_equal(Point a, Point b)
{
	return a.x == b.x && a.y == b.y;
}

Node* lowest_f_cost_node(Node** open_set, int open_set_size)
{
	Node* lowest_f_cost_node;
	int i;
	lowest_f_cost_node = open_set[0];
	for (i = 1; i < open_set_size; i++) {
		if (open_set[i]->f_cost < lowest_f_cost_node->f_cost) {
			lowest_f_cost_node = open_set[i];
		}
	}
	return lowest_f_cost_node;
}

int in_set(Node** node_set, int set_size, Point point)
{
	int i;
	for (i = 0; i < set_size; i++) {
		if (points_are_equal(node_set[i]->point, point)) {
			return 1;
		}
	}
	return 0;
}

void remove_node_from_set(Node** node_set, int* set_size, Point point)
{
	int i;
	for (i = 0; i < *set_size; i++) {
		if (points_are_equal(node_set[i]->point, point)) {
			node_set[i] = node_set[--(*set_size)];
			return;
		}
	}
}

Node* find_node(Node** node_set, int set_size, Point point)
{
	int i;
	for (i = 0; i < set_size; i++) {
		if (points_are_equal(node_set[i]->point, point)) {
			return node_set[i];
		}
	}
	return NULL;
}

Node** get_neighbors(Node* node, char** grid, int width, int height)
{
	int i;
	Node** neighbors = (Node**)calloc(5, sizeof(Node*));
	int idx = 0;

	Point directions[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

	for (i = 0; i < 4; i++) {
		int x = node->point.x + directions[i].x;
		int y = node->point.y + directions[i].y;

		if (x >= 0 && x < width && y >= 0 && y < height && grid[y][x] != '.') {
			Node* neighbor = (Node*)malloc(sizeof(Node));
			neighbor->point.x = x;
			neighbor->point.y = y;
			neighbor->g_cost = 0;
			neighbor->h_cost = 0;
			neighbor->f_cost = 0;
			neighbor->parent = NULL;

			neighbors[idx++] = neighbor;
		}
	}

	return neighbors;
}

void free_nodes(Node** nodes, int node_count)
{
	int i;
	for (i = 0; i < node_count; i++) {
		free(nodes[i]);
	}
	free(nodes);
}

Node* a_star(Point start, Point end, char** grid, int width, int height)
{
	Node* start_node;
	Node** open_set;
	Node** closed_set;
	int open_set_size;
	int closed_set_size;
	start_node = (Node*)malloc(sizeof(Node));
	start_node->point = start;
	start_node->g_cost = 0;
	start_node->h_cost = manhattan_distance(start, end);
	start_node->f_cost = start_node->h_cost;
	start_node->parent = NULL;

	open_set = (Node**)malloc(width * height * sizeof(Node*));
	open_set_size = 0;
	open_set[open_set_size++] = start_node;

	closed_set = (Node**)malloc(width * height * sizeof(Node*));
	closed_set_size = 0;

	while (open_set_size > 0) {
		Node** neighbors;
		int neighbor_count;
		int i;
		Node* current_node = lowest_f_cost_node(open_set, open_set_size);

		if (points_are_equal(current_node->point, end)) {
			return current_node;
		}

		remove_node_from_set(open_set, &open_set_size, current_node->point);
		closed_set[closed_set_size++] = current_node;

		neighbors = get_neighbors(current_node, grid, width, height);
		neighbor_count = 0;
		while (neighbors[neighbor_count] != NULL) {
			neighbor_count++;
		}
		for (i = 0; i < neighbor_count; i++) {
			int tentative_g_cost;
			Node* neighbor = neighbors[i];

			if (in_set(closed_set, closed_set_size, neighbor->point)) {
				free(neighbor);
				continue;
			}
			tentative_g_cost = current_node->g_cost + 1;

			if (!in_set(open_set, open_set_size, neighbor->point)) {
				open_set[open_set_size++] = neighbor;
			} else if (tentative_g_cost >= neighbor->g_cost) {
				free(neighbor);
				continue;
			}

			neighbor->parent = current_node;
			neighbor->g_cost = tentative_g_cost;
			neighbor->h_cost = manhattan_distance(neighbor->point, end);
			neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
		}

		free(neighbors);
	}

	free_nodes(open_set, open_set_size);
	free_nodes(closed_set, closed_set_size);

	return NULL;
}

Point get_acceleration(Node* path)
{
	Point acceleration = { 0, 0 };

	if (path != NULL && path->parent != NULL) {
		acceleration.x = path->parent->point.x - path->point.x;
		acceleration.y = path->parent->point.y - path->point.y;
	}

	return acceleration;
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
	Point start;
	Point end;
	int x;
	int y;
	Node* path;
	Point acceleration;

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
		start.x = myX;
		start.y = myY;
		end.x = -1;
		end.y = -1;
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				if (grid[y][x] == '=') {
					end.x = x;
					end.y = y;
					break;
				}
			}
			if (end.x != -1) {
				break;
			}
		}
		path = a_star(start, end, grid, width, height);
		acceleration = get_acceleration(path);
		accelerationX = acceleration.x;
		accelerationY = acceleration.y;
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
		free_nodes(&path, 0);
	}
	/* Free memory */
	for (row = 0; row < height; ++row) {
		free(grid[row]);
	}
	free(grid);

	return EXIT_SUCCESS;
}
