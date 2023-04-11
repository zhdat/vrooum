#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

typedef struct Node {
	int x, y;			 /* Position du nœud dans la grille */
	int g_cost;			 /* Coût du chemin depuis le nœud de départ jusqu'à ce nœud */
	int h_cost;			 /* Coût estimé du chemin de ce nœud jusqu'au nœud d'arrivée (heuristique) */
	int f_cost;			 /* Somme des coûts g et h */
	struct Node* parent; /* Nœud parent dans le chemin */
} Node;

typedef struct NodeList {
	Node* node;
	struct NodeList* next;
} NodeList;

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

Node* create_node(int x, int y, Node* parent)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->x = x;
	new_node->y = y;
	new_node->g_cost = 0;
	new_node->h_cost = 0;
	new_node->f_cost = 0;
	new_node->parent = parent;
	return new_node;
}

int heuristic_cost(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

int is_valid_position(int x, int y, int width, int height, char** grid)
{
	return x >= 0 && x < width && y >= 0 && y < height && (grid[y][x] == '#' || grid[y][x] == '~' || grid[y][x] == '=');
}

NodeList* create_node_list(Node* node)
{
	NodeList* new_list = (NodeList*)malloc(sizeof(NodeList));
	new_list->node = node;
	new_list->next = NULL;
	return new_list;
}

void add_to_list(NodeList** list, Node* node)
{
	NodeList* new_list = create_node_list(node);
	new_list->next = *list;
	*list = new_list;
}

int is_in_list(NodeList* list, Node* node)
{
	NodeList* current = list;
	while (current != NULL) {
		if (current->node->x == node->x && current->node->y == node->y) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

void remove_from_list(NodeList** list, Node* node)
{
	NodeList* current = *list;
	NodeList* previous = NULL;
	while (current != NULL) {
		if (current->node->x == node->x && current->node->y == node->y) {
			if (previous != NULL) {
				previous->next = current->next;
			} else {
				*list = current->next;
			}
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

Node* get_lowest_f_cost_node(NodeList* list)
{
	NodeList* current = list;
	Node* lowest_node = NULL;
	while (current != NULL) {
		if (lowest_node == NULL || current->node->f_cost < lowest_node->f_cost) {
			lowest_node = current->node;
		}
		current = current->next;
	}
	return lowest_node;
}

void free_list(NodeList* list)
{
	NodeList* current = list;
	while (current != NULL) {
		NodeList* next = current->next;
		free(current);
		current = next;
	}
}

Node* a_star(int start_x, int start_y, int end_x, int end_y, char** grid, int width, int height)
{
	int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	NodeList* open_list = NULL;
	NodeList* closed_list = NULL;

	Node* start_node = create_node(start_x, start_y, NULL);
	add_to_list(&open_list, start_node);

	Node* neighbor;

	while (open_list != NULL) {
		int i;
		int y;
		Node* current_node = get_lowest_f_cost_node(open_list);
		remove_from_list(&open_list, current_node);
		add_to_list(&closed_list, current_node);

		if (current_node->x == end_x && current_node->y == end_y) {
			Node* result = create_node(current_node->x, current_node->y, current_node->parent);
			free_list(open_list);
			free_list(closed_list);
			return result;
		}

		for (i = 0; i < 8; ++i) {
			int new_x = current_node->x + dx[i];
			int new_y = current_node->y + dy[i];

			if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) {
				if (grid[new_y][new_x] == '.' || grid[new_y][new_x] == '=' || is_in_list(closed_list, create_node(new_x, new_y, NULL))) {
					continue;
				}

				neighbor = create_node(new_x, new_y, current_node);
				int tentative_g_cost = current_node->g_cost + 1;

				if (!is_in_list(open_list, neighbor) || tentative_g_cost < neighbor->g_cost) {
					neighbor->parent = current_node;
					neighbor->g_cost = tentative_g_cost;
					neighbor->h_cost = heuristic_cost(new_x, new_y, end_x, end_y);
					neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

					if (!is_in_list(open_list, neighbor)) {
						add_to_list(&open_list, neighbor);
					}
				}
			}
		}
	}
	return closed_list->next->node;
	free_list(open_list);
	free_list(closed_list);
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
	Node* path;
	int depart;
	depart = 1;

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
		/* Trouver la position d'arrivée */
		int finish_x, finish_y, x;
		for (y = 0; y < height; ++y) {
			for (x = 0; x < width; ++x) {
				if (grid[y][x] == '=') {
					finish_x = x;
					finish_y = y;
					break;
				}
			}
		}

		/* Utiliser l'algorithme A* pour trouver le chemin le plus court */
		Node* last_node = a_star(myX, myY, finish_x, finish_y, grid, width, height);
		Node* first_move = NULL;
		if (last_node != NULL) {
			Node* current_node = last_node;
			while (current_node->parent != NULL && current_node->parent->parent != NULL) {
				current_node = current_node->parent;
			}
			first_move = current_node;
		}

		/* Utiliser les coordonnées de first_move pour déterminer le mouvement */
		if (first_move != NULL) {
			accelerationX = first_move->x - myX;
			accelerationY = first_move->y - myY;
		} else {
			accelerationX = 0;
			accelerationY = 0;
		}

		/* Libérer les nœuds du chemin */
		Node* current_node = last_node;
		while (current_node != NULL) {
			Node* tmp_node = current_node;
			current_node = current_node->parent;
			free(tmp_node);
		}

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
