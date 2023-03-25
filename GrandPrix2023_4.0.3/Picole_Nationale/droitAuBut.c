#include "droitAuBut.h"
#include "queue.h"
#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

/**
 * @brief Computes the Euclidean distance between two points.
 *
 * @param x1 x coordinate of the first point
 * @param y1 y coordinate of the first point.
 * @param x2 x coordinate of the second point.
 * @param y2 y coordinate of the second point.
 * @return The Euclidean distance between the input points
 */
double distance(double x1, double y1, double x2, double y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

/**
 * @brief Function to create a new node of an adjacency list.
 *
 * @param dest The destination vertex.
 * @param next Pointer of the following node in the adjacency list.
 * @return A pointer to the new node.
 */
NodeList *newNodeGraph(int dest, NodeList *next) {
  NodeList *newNode = (NodeList *)malloc(sizeof(NodeList));
  newNode->dest = dest;
  newNode->next = next;
  return newNode;
}

/**
 * @brief Function to add an edge in a graph.
 *
 * @param vertex1 First vertex of the edge.
 * @param vertex2 Second vertex of the edge.
 * @param graph The graph in which the edge is added.
 *
 * Only the array of adjacency lists is updated.
 */
void addEdgeInGraph(Graph graph, int vertex1, int vertex2) {
  graph.array[vertex1] = newNodeGraph(vertex2, graph.array[vertex1]);
  graph.array[vertex2] = newNodeGraph(vertex1, graph.array[vertex2]);
}

/**
 * @brief Function to create a random graph with a specified number of vertices
 *and parameter sigma.
 *
 *
 * @param numVertices The number of vertices in the graph.
 * @param sigma Parameter of the procedure to generate a random graph.
 *
 * @return A pointer to the new graph.
 *
 * The elements of the array parents are set to -1.
 * The array of adjacency lists must be updated.
 */
Graph createGraph(int numVertices, double sigma) {
  Graph graph;
  int i;
  int j;
  graph.numberVertices = numVertices;
  graph.array = (NodeList **)malloc(numVertices * sizeof(NodeList *));
  graph.xCoordinates = (double *)malloc(numVertices * sizeof(double));
  graph.yCoordinates = (double *)malloc(numVertices * sizeof(double));
  graph.sigma = sigma;
  graph.parents = (int *)malloc(numVertices * sizeof(int));
  for (i = 0; i < numVertices; i++) {
    graph.array[i] = NULL;
    graph.parents[i] = -1;
    graph.xCoordinates[i] = (double)rand() / RAND_MAX;
    graph.yCoordinates[i] = (double)rand() / RAND_MAX;
  }
  for (i = 0; i < numVertices; i++) {
    for (j = i + 1; j < numVertices; j++) {
      if (distance(graph.xCoordinates[i], graph.yCoordinates[i],
                   graph.xCoordinates[j], graph.yCoordinates[j]) < sigma) {
        addEdgeInGraph(graph, i, j);
      }
    }
  }
  return graph;
}

void Astar(Graph graph, int source, int destination) {
  int nbVertices = graph.numberVertices;
  int i;
  double cost;
  double heuristic;
  NodeList *neighbor;
  Queue *queue;
  /* Initialize the arrays for visited nodes and distances */
  int *visited = (int *)malloc(nbVertices * sizeof(int));
  double *distances = (double *)malloc(nbVertices * sizeof(double));
  for (i = 0; i < nbVertices; i++) {
    visited[i] = 0;
    distances[i] = INFINITY;
  }
  /* Set the distance of the source node to 0 */
  distances[source] = 0;

  /* Initialize the priority queue with the source node */
  queue = createQueue(nbVertices);
  enqueue(queue, source);

  /* Start the A* algorithm */
  while (!isQueueEmpty(*queue)) {
    /* Pop the node with the lowest total cost from the priority queue */
    int currentNode = dequeue(queue);
    visited[currentNode] = 1;

    /* If the destination node has been reached, stop the algorithm */
    if (currentNode == destination) {
      break;
    }

    /* Visit all the neighbors of the current node */
    neighbor = graph.array[currentNode];
    while (neighbor != NULL) {
      int neighborIndex = neighbor->dest;

      /* If the neighbor has already been visited */
      if (visited[neighborIndex] == 1) {
        neighbor = neighbor->next;
        continue;
      }

      /* Compute the distance between the current node and the neighbor */
      cost = distance(
          graph.xCoordinates[currentNode], graph.yCoordinates[currentNode],
          graph.xCoordinates[neighborIndex], graph.yCoordinates[neighborIndex]);
      cost += distances[currentNode];

      /* Compute the estimated cost to the destination node */
      heuristic = distance(
          graph.xCoordinates[neighborIndex], graph.yCoordinates[neighborIndex],
          graph.xCoordinates[destination], graph.yCoordinates[destination]);

      /* Update the distance of the neighbor if a shorter paths has been found
       */
      if (cost + heuristic < distances[neighborIndex]) {
        distances[neighborIndex] = cost + heuristic;
        graph.parents[neighborIndex] = currentNode;
        enqueue(queue, neighborIndex);
      }

      neighbor = neighbor->next;
    }
  }
}

/* How to use */
/* Graph graph = createGraph(numVertices, sigma);
int source = 0;
int destination = numVertices - 1;
Astar(graph, source, destination); */

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
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand) {
  int gas = accX * accX + accY * accY;
  gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
  if (inSand) {
    gas += 1;
  }
  return -gas;
}

int main() {
  int row;
  int width, height;
  int gasLevel;
  int boosts = BOOSTS_AT_START;
  int round = 0;
  int accelerationX = 1, accelerationY = 0;
  int speedX = 0, speedY = 0;
  char action[100];
  char line_buffer[MAX_LINE_LENGTH];

  boosts = boosts; /* Prevent warning "unused variable" */
  fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
  sscanf(line_buffer, "%d %d %d", &width, &height, &gasLevel);
  fprintf(stderr, "=== >Map< ===\n");
  fprintf(stderr, "Size %d x %d\n", width, height);
  fprintf(stderr, "Gas at start %d \n\n", gasLevel);
  for (row = 0; row < height; ++row) { /* Read map data, line per line */
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    fputs(line_buffer, stderr);
  }
  fflush(stderr);
  fprintf(stderr, "\n=== Race start ===\n");
  while (!feof(stdin)) {
    int myX, myY, secondX, secondY, thirdX, thirdY;
    round++;
    fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);
    fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots */
    sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY,
           &thirdX, &thirdY);
    fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n", myX, myY,
            secondX, secondY, thirdX, thirdY);
    fflush(stderr);
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
      int *p = NULL;
      fprintf(stderr, "Good Bye!\n");
      fflush(stderr);
      *p = 0;
    }
  }

  return EXIT_SUCCESS;
}
