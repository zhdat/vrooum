/**
 * @file stack.h
 * @brief Header file for the Stack data structure.
 */

#ifndef STACK_H
#define STACK_H

/**
 * @brief A node in the Stack data structure.
 */
typedef struct nodeStack {
    int data; /** The data held by the node. */
    struct nodeStack* next; /** A pointer to the next node in the list. */
} NodeStack;

/**
 * @brief The Stack data structure.
 */
typedef struct stack {
    NodeStack* top; /** A pointer to the top (first) element in the stack. */
} Stack;


/**
 * @brief Creates a new node with the specified data and next node.
 *
 * @param data The data to store in the node.
 * @param next A pointer to the next node in the list.
 *
 * @return A pointer to the new node.
 */
NodeStack* createNode(int data, NodeStack* next);


/**
 * @brief Creates a new stack with no elements.
 *
 * @return A pointer to the new stack.
 */
Stack* createStack();

/**
 * @brief Pushes an element onto the top of the stack.
 *
 * @param stack The stack to push onto.
 * @param data The data to push onto the stack.
 */
void push(Stack* stack, int data);

/**
 * @brief Pops an element from the top of the stack.
 *
 * @param stack The stack to pop from.
 *
 * @return The data from the top element of the stack.
 */
int pop(Stack* stack);

/**
 * @brief Peeks at the top element of the stack without removing it.
 *
 * @param stack The stack to peek at.
 *
 * @return The data from the top element of the stack.
 */
int peek(Stack stack);

/**
 * @brief Checks whether the stack is empty.
 *
 * @param stack The stack to check.
 *
 * @return 1 if the stack is empty, 0 otherwise.
 */
int isStackEmpty(Stack stack);

/**
 * @brief Prints the contents of the stack to stdout.
 *
 * @param stack The stack to print.
 */
void stackPrint(Stack stack);

#endif /* STACK_H */
