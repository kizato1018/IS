#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Child-thread
void* child(void* data) 
{
    char *str = (char*) data; 
    for(int i = 0;i < 10; i++) 
    {
        printf("%s\n", str); 
        sleep(1);
    }
    pthread_exit(NULL); // Exit
}

// Main
int main() 
{
    pthread_t t; // pthread variable
    pthread_create(&t, NULL, child, "Child"); // set up the pthread variable with a function pointer (task)

    // main thread
    for(int i = 0;i < 10; i++) 
    {
        printf("Master\n");
        sleep(1);
    }

    pthread_join(t, NULL); // Wait for the thread termination
    return 0;
}