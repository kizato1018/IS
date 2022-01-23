#include <stdio.h>      // for printf() and fprintf() 
#include <sys/socket.h> // for socket(), connect(), send(), and recv() 
#include <arpa/inet.h>  // for sockaddr_in and inet_addr() 
#include <stdlib.h>     // for atoi() and exit() 
#include <string.h>     // for memset() 
#include <unistd.h>     // for close() 

#define STRBUFSIZE 1024 // Size of receive buffer 

int main(int argc, char *argv[])
{
    int sock;                        // Socket descriptor 
    struct sockaddr_in echoServAddr; // Echo server address 
    unsigned short echoServPort;     // Echo server port 
    char *servIP;                    // Server IP address (dotted quad)
    char echoString[STRBUFSIZE];     // Buffer for echo string  
    char echoBuffer[STRBUFSIZE];     // Buffer for echo string 
    unsigned int echoStringLen;      // Length of string to echo 
    int bytesRcvd, totalBytesRcvd;   // Bytes read in single recv() and total bytes read 

    if ( ( argc < 2 ) || (argc > 3 ) )    // Test for correct number of arguments 
    {
       fprintf( stderr, "Usage: %s <Server IP> [<Echo Port>]\n", argv[0] );
       exit(1);
    }

    servIP = argv[1];             // First arg: server IP address (dotted quad) 

    if ( argc == 3 )
    {
        // Use given port, if any 
        echoServPort = atoi(argv[2]); 
    }
    else
    {
        // 7 is the well-known port for the echo service
        echoServPort = 7;  
    } 

    // Create a socket using UDP 
    if ( ( sock = socket(AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        perror("socket() failed");
        exit(1);
    }
    
    // Construct the server address structure 
    memset(&echoServAddr, 0, sizeof(echoServAddr));     // Zero out structure 
    echoServAddr.sin_family      = AF_INET;             // Internet address family 
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   // Server IP address 
    echoServAddr.sin_port        = htons(echoServPort); // Server port 

    while ( 1 )
    {
        bzero( echoString, STRBUFSIZE );
        bzero( echoBuffer, STRBUFSIZE );
        
        printf("Enter(q for exit): \n"); 
        
        fgets( echoString, STRBUFSIZE, stdin ); 
        
        echoStringLen = strlen( echoString );
        
        if ( echoStringLen == 2 && echoString[0] == 'q' && echoString[1] == '\n' )
        {
            break;
        }
        
        if ( sendto(sock, echoString, echoStringLen, 0, (const struct sockaddr *) &echoServAddr, sizeof( echoServAddr ) ) != echoStringLen )
        {
            perror( "sendto() sent a different number of bytes than expected");
            exit(1);
        }
        
        printf("Received"); 
        
        int len = sizeof( echoServAddr );
        if ( ( bytesRcvd = recvfrom( sock, echoBuffer, STRBUFSIZE, 0, (struct sockaddr *) &echoServAddr, &len ) ) < 0 )
        {
            perror("recvfrom() failed or connection closed prematurely");
            exit(1);
        }
        printf( "(%d): ", bytesRcvd );
        printf( "%s", echoBuffer);      // Print the echo buffer 
        
    }
    
    printf("\n");    // Print a final linefeed 

    close(sock);
    return 0;
}