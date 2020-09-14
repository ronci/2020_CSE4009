#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int compare(int a, int b) {
	if (a > b)
		return 1;
	else
		return -1;
}

int main() {
	int (*ascending)() = compare;
	int n_array[SIZE] = { 2,-9, 10, 15, 1, 3, -12, 5, 4, 1 };
	int i, j;
	int temp;
	for (i = SIZE - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (ascending(n_array[j], n_array[j + 1]) == 1) {
				temp = n_array[j];
				n_array[j] = n_array[j + 1];
				n_array[j + 1] = temp;
			}
		}
	}
	for (i = 0; i < SIZE; i++) {
		printf("%d ", n_array[i]);
	}
	printf("\n");
	return 0;
}

