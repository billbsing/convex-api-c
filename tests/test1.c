

#include <stdio.h>
#include "convex.h"

int main (int argc, char *argv) {
	convex_p convex;
	if (!convex_init(&convex)) {
		printf("failed to load convex");
		return 2;
	}
	convex_close(convex);
	return 0;
}

