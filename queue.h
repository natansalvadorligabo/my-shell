#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_MAX_SIZE 10

typedef char* Element;
typedef struct queue* Queue;

void init(Queue* queue);

bool is_full(Queue queue);

bool is_empty(Queue queue);

bool enqueue(Queue queue, Element new_element);

Element dequeue(Queue queue);

int size(Queue queue);