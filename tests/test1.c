

#include <stdio.h>
#include <convex.h>

int main() {
	convex_p convex;
	int result = convex_init(&convex);
	if (result) {
		printf("failed to load conve %d\n", result);
		return 2;
	}
	convex_close(convex);
	return 0;
}

