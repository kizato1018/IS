#include <stdio.h>
#include <stdlib.h>

int glo_v = 0;
static int glo_ustv; 

void func(int arg) {
	int func_v;
	static int func_stv = 0;
	printf("Argument in function Address: %p\n", &arg);
	printf("Local Variable in function Address: %p\n", &func_v); 
	printf("Local Static Variable in function Address: %p\n", &func_stv);
	return;
}

int main() {
	int loc_v;
	static int loc_stv = 0;
	static int loc_ustv;
	int *malloc_v = malloc(sizeof(int));
	
	printf("Local Variable Address: %p\n", &loc_v);
	printf("Local Static Initialized Variable Address: %p\n", &loc_stv);
	printf("Local Static Uninitialized Variable Address: %p\n", &loc_ustv);
	printf("Global Initialized Variable Address: %p\n", &glo_v);
	printf("Global Static Uninitialized Variable Address: %p\n", &glo_ustv);
	printf("Memory Allocation Address: %p\n", malloc_v);
	printf("Function Address: %p\n", &func);
	func(0);
	
	return 0;
}
