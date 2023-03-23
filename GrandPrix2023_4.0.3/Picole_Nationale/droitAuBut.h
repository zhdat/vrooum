//
// Created by calliste Ravix on 20/03/2023.
//

#ifndef VROOUM_DROITAUBUT_H
#define VROOUM_DROITAUBUT_H

/* Algorithm A start */
typedef struct node {
    int x; /* x coordinate */
    int y; /* y coordinate */
    int distance_start; /* distance from start node */
    int distance_heuristic; /* heuristic distance to end node */
    int distance_total; /* total distance (distance_start + distance_heuristic) / quality of the node */
    struct node* parent; /* pointer to parent node */
} Node;

typedef struct list {
    Node* node; /* pointer to node */
    struct list* next; /* pointer to next list element */
} List;

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

Node* createNode(int x, int y, int distance_start, int distance_heuristic, Node* parent);

List* createList(Node* node, List* next);

void freeList(List* list);

void freeNode(Node* node);

int distanceHeuristic(Node* node, Node* end);
#endif //VROOUM_DROITAUBUT_H
