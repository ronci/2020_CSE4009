#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int nData;
	struct Node* next;
}Node;

typedef struct Stack {
	Node* top;
}Stack;

void InitializeStack(Stack* stack);
void Push(Stack* stack, int nData);
int Pop(Stack* stack);
void Clear(Stack* stack);


int main(void) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	InitializeStack(stack);
	Push(stack, 2);
	if (Pop(stack) == 0) {
		printf("No Data\n");
	}
	Clear(stack);
	free(stack);
	return 0;
}

void InitializeStack(Stack* stack) {
	stack->top = NULL;
}

void Push(Stack* stack, int nData) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->nData = nData;
	if (stack->top == NULL) {
		stack->top = newNode;
		newNode->next = NULL;
	}
	else {
		newNode->next = stack->top;
		stack->top = newNode;
	}
}

int Pop(Stack* stack) {
	if (stack->top == NULL) {
		return 0;
	}
	Node* nowNode = stack->top;
	stack->top = nowNode->next;
	free(nowNode);
	return 1;
}

void Clear(Stack* stack) {
	Node* nowNode = stack->top;
	Node* nextNode;
	while (nowNode != NULL) {
		nextNode = nowNode->next;
		free(nowNode);
		nowNode = nextNode;
	}
}


