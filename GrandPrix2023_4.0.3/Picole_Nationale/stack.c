/**
 * @file stack.c
 * @brief Implementation file for the Stack data structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/**
 * @brief Creates a new node with the specified data and next node.
 *
 * @param data The data to store in the node.
 * @param next A pointer to the next node in the list.
 *
 * @return A pointer to the new node.
 */
NodeStack* createNode(int data, NodeStack* next) {
    NodeStack* newNode = (NodeStack*) malloc(sizeof(NodeStack));
    newNode->data = data;
    newNode->next = next;
    return newNode;
}

/**
 * @brief Creates a new stack with no elements.
 *
 * @return A pointer to the new stack.
 */
Stack* createStack() {
    Stack* newStack = (Stack*) malloc(sizeof(Stack));
    newStack->top = NULL;
    return newStack;
}

/**
 * @brief Pushes an element onto the top of the stack.
 *
 * @param stack The stack to push onto.
 * @param data The data to push onto the stack.
 */
void push(Stack* stack, int data) {
    NodeStack* newNode = createNode(data, stack->top);
    stack->top = newNode;
    return;
}

/**
 * @brief Pops an element from the top of the stack.
 *
 * @param stack The stack to pop from.
 *
 * @return The data from the top element of the stack.
 */
int pop(Stack* stack) {
    if (isStackEmpty(*stack) == 1){
        printf("Stack is empty");
        return -1;
    }
    int data = stack->top->data;
    NodeStack* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

/**
 * @brief Peeks at the top element of the stack without removing it.
 *
 * @param stack The stack to peek at.
 *
 * @return The data from the top element of the stack.
 */
int peek(Stack stack) {
    if (isStackEmpty(stack) == 1){
        printf("Stack is empty");
        return -1;
    }
    return stack.top->data;
}

/**
 * @brief Checks whether the stack is empty.
 *
 * @param stack The stack to check.
 *
 * @return 1 if the stack is empty, 0 otherwise.
 */
int isStackEmpty(Stack stack) {
    if (stack.top == NULL) {
        return 1;
    }
    return 0;
}

/**
 * @brief Prints the contents of the stack to stdout.
 *
 * @param stack The stack to print.
 */
void stackPrint(Stack stack) {
    printf("[");
    NodeStack* current = stack.top;
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
