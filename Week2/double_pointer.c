#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char** pp_data = NULL;
	int n;
	int length;
	int i;

	scanf("%d", &n);
	pp_data = (char**)malloc(sizeof(char*) * n);
	scanf("%d", &length);
	for (i = 0; i < n; i++) {
		pp_data[i] = (char*)malloc(sizeof(char) * length);
		scanf("%s", pp_data[i]);
	}
	for (i = 0; i < n; i++) {
		printf("%s\n", pp_data[i]);
	}

	for (i = 0; i < n; i++) {
		free(pp_data[i]);
	}
	free(pp_data);

	return 0;
}

