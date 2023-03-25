#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 * @brief Creates a new, empty queue.
 *
 * @return A pointer to the newly created queue.
 */
 Queue* createQueue() {
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

/**
 * @brief Prints all the elements in the queue, from front to rear.
 *
 * @param q The queue to print.
 */
void queuePrint(Queue q) {
    printf("[");
    nodeQueue* current = q.front;
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
        if (current != NULL) {
            printf("\t");
        }
    }
    printf("]\n");
    return;
}


/**
 * @brief Adds an element to the rear of the queue.
 *
 * @param q A pointer to the queue to add the element to.
 * @param data The data to be added to the queue.
 */
void enqueue(Queue* q, int data) {
    nodeQueue* newNode = (nodeQueue*) malloc(sizeof(nodeQueue));
    newNode->data = data;
    newNode->next = NULL;
    if (q->front == NULL) {
        q->front = newNode;
    } else {
        q->rear->next = newNode;
    }
    q->rear = newNode;
    return;
}

/**
 * @brief Removes and returns the front element of the queue.
 *
 * @param q A pointer to the queue to remove the front element from.
 * @return The data stored in the front element of the queue.
 */
int dequeue(Queue* q) {
    nodeQueue* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    free(temp);
    return data;
}

/**
 * @brief Checks whether the queue is empty.
 *
 * @param q The queue to check.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int isQueueEmpty(Queue q) {
    if (q.front == NULL) {
        return 1;
    }
    return 0;
}

/**
 * @brief Returns the value of the front node but doesn't modify the queue
 *
 * @param q A pointer to the queue to get the size of.
 * @return The number of elements in the queue.
 */
int queueGetFrontValue(Queue q) {
    if (isQueueEmpty(q) == 1)
        return -1;
    return q.front->data;
}
