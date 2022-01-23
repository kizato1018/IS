#include <stdio.h>
#include <stdlib.h>

int main() {
	char* shell =  getenv("MYSHELL");
	char* arg = getenv("MYARG");
	if (shell)
		printf("0x%x\n", (unsigned int)shell);
	if (arg)
		printf("0x%x\n", (unsigned int)arg);
	return 0;
}
