#include <stdio.h>
#include <unistd.h>

int main() {
	char gdb_path[] = "/usr/bin/gdb";
	char test_path[] = "/home/seed/HW3/display_env";
	char *gdb_argv[] = {gdb_path, test_path, NULL};
	char *test_argv[] = {test_path, NULL};
	
	// execve(test_path, test_argv, NULL);
	execve(gdb_path, gdb_argv, NULL);
	
	return 0;
}
