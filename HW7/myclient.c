#include <stdio.h>      // for printf() and fprintf() 
#include <sys/socket.h> // for socket(), connect(), send(), and recv() 
#include <arpa/inet.h>  // for sockaddr_in and inet_addr() 
#include <stdlib.h>     // for atoi() and exit() 
#include <string.h>     // for memset() 
#include <unistd.h>     // for close() 
#include <stddef.h>
#include <sys/un.h>

#define STRBUFSIZE 1024 // Size of receive buffer 

int main(int argc, char *argv[])
{
    int sock;                        // Socket descriptor 
    struct sockaddr_un echoServerUn; // Echo server 
    char *servername;                // Server file name
    char *clientname;		    // Client file name
    char echoString[STRBUFSIZE];     // Buffer for echo string  
    char echoBuffer[STRBUFSIZE];     // Buffer for echo string 
    unsigned int echoStringLen;      // Length of string to echo 
    int bytesRcvd, totalBytesRcvd;   // Bytes read in single recv() and total bytes read 
    int len;
    if ( argc != 2 )    // Test for correct number of arguments 
    {
       fprintf( stderr, "Usage: %s <Server Name>\n", argv[0] );
       exit(1);
    }

    servername = argv[1];             // First arg: server file name
    clientname = "tmp.sock";


    // Create a reliable, stream socket using TCP 
    if ( ( sock = socket(AF_UNIX, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror("socket() failed");
        exit(1);
    }
    
    // Bind to the client file
    memset(&echoServerUn, 0, sizeof(echoServerUn));
    echoServerUn.sun_family = AF_UNIX;
    strcpy(echoServerUn.sun_path, clientname); 
    len = offsetof(struct sockaddr_un, sun_path) + strlen(clientname);
    unlink(echoServerUn.sun_path);				// in case it already exists  
    if ( bind(sock, (struct sockaddr *)&echoServerUn, len) < 0 ) {
        perror( "bind() failed" );
        exit(1);
    }
    printf("Client binded.\n");
    
    memset(&echoServerUn, 0, sizeof(echoServerUn));
    echoServerUn.sun_family = AF_UNIX;
    strcpy(echoServerUn.sun_path, servername);    
    len = offsetof(struct sockaddr_un, sun_path) + strlen(servername);
    
	// Establish the connection to the echo server
    if ( connect( sock, (struct sockaddr *)&echoServerUn, len ) < 0 )
    {
        perror( "connect() failed" );
        exit(1);
    }
    printf("Client Connected.\n");
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
        
        if ( send(sock, echoString, echoStringLen, 0) != echoStringLen )
        {
            perror( "send() sent a different number of bytes than expected");
            exit(1);
        }
        
        printf("Received"); 
        
        if ( ( bytesRcvd = recv( sock, echoBuffer, STRBUFSIZE, 0) ) <= 0 )
        {
            perror("recv() failed or connection closed prematurely");
            exit(1);
        }
        printf( "(%d): ", bytesRcvd );
        printf( "%s", echoBuffer);      // Print the echo buffer 
        
    }
    
    printf("\n");    // Print a final linefeed 

    close(sock);
    return 0;
}
