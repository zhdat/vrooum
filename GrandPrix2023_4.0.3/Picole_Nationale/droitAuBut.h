
#ifndef VROOUM_DROITAUBUT_H
#define VROOUM_DROITAUBUT_H

#include <stdio.h>
#include <stdlib.h>

/* Algorithm A star */
typedef struct nodeList
{
  int dest;              /** The index of the destination vertex. */
  struct nodeList *next; /** Pointer to the next node in the list. */
} NodeList, *List;

/**
 * @brief This struct represents the graph using an array of adjacency lists.
 *
 */
typedef struct Graph
{
  int numberVertices;   /** The number of vertices in the graph. */
  List *array;          /** Array of adjacency lists. A null pointer means an empty list*/
  double *xCoordinates; /** array of the x coordinates of the points of each vertex */
  double *yCoordinates; /** array of the y coordinates of the points of each vertex */
  double sigma;         /** maximal distance between two neighboring vertices */
  int *parents;         /** array to store the parent of each node for the covering tree. */
} Graph;

/**
 * @brief Function to create a new node of an adjacency list.
 *
 * @param dest The destination vertex.
 * @param next Pointer of the following node in the adjacency list.
 * @return A pointer to the new node.
 */
NodeList *newNodeGraph(int dest, NodeList *next);

/**
 * @brief Function to add an edge in a graph.
 *
 * @param vertex1 First vertex of the edge.
 * @param vertex2 Second vertex of the edge.
 * @param graph The graph in which the edge is added.
 *
 * Only the array of adjacency lists is updated.
 */
void addEdgeInGraph(Graph graph, int vertex1, int vertex2);

/**
 * @brief Function to create a random graph with a specified number of vertices
 * and parameter sigma.
 *
 * @param numVertices The number of vertices in the graph.
 * @param sigma Parameter of the procedure to generate a random graph.
 *
 * @return A pointer to the new graph.
 *
 * The elements of the array parents are set to -1.
 * The array of adjacency lists must be updated.
 */
Graph createGraph(int numVertices, double sigma);

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

#endif /* VROOUM_DROITAUBUT_H */
