#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// the max connection number of the server
#define MAX_CONNECTION_NUMBER 5
#define MAXPENDING 5        // Maximum outstanding connection requests
#define RCVBUFSIZE 1000	    // Buffer Size

/* * Create a server endpoint of a connection. * Returns fd if all OK, <0 on error. */
int  CreateUnixServerSocket(char *servername) {
	int sock;
	int len, rval; 
	struct sockaddr_un echoServerUn, un;
	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
  		return(-1); 
    }
    
    memset(&echoServerUn, 0, sizeof(echoServerUn));
    echoServerUn.sun_family = AF_UNIX;
    strcpy(echoServerUn.sun_path, servername); 
    len = offsetof(struct sockaddr_un, sun_path) + strlen(servername);
    unlink(servername);				// in case it already exists  
    if ( bind(sock, (struct sockaddr *)&echoServerUn, len) < 0 ) {
        perror( "bind() failed" );
        exit(1);
    }
    printf("Unix Socket binded.\n");
    printf("Unix Socket listening...\n");
    // Mark the socket so it will listen for incoming connections 
    if ( listen( sock, MAXPENDING ) < 0 )
    {
        perror( "listen() failed" );
        exit(1);
    }
    return sock;
}

int AcceptUnixClient(int serverSock, struct sockaddr_un *echoClientUn) {
    int clientSock;        			// Socket descriptor for client
    unsigned int clientLen;     	// Length of client address data structure 
    clientLen = sizeof(echoClientUn); 
    // Wait for a client to connect 
    if ( ( clientSock = accept( serverSock, (struct sockaddr *) echoClientUn, &clientLen ) ) < 0 ) {
        perror("accept() failed");
        exit(1);
    }
    printf("Accept Unix Cliend.\n");
    return clientSock;
}

int HandleUnixClient( int socket ) {
    char    echoBuffer[RCVBUFSIZE] = {0};  // Buffer for echo string 
    int     recvMsgSize;                   // Size of received message 
    
    // Receive message from client 
    if ( ( recvMsgSize = recv( socket, echoBuffer, RCVBUFSIZE, 0 ) ) < 0 ) {
        perror("recv() failed");
        exit(1);
    }

    // Send received string and receive again until end of transmission 
    if ( recvMsgSize > 0 ) {      // zero indicates end of transmission
        printf( "From FD (%d) recv %d bytes:\n\tData:", socket, recvMsgSize );
        
        for( int i = 0 ; i < recvMsgSize ; i++ )
        {
            printf( "%02X ", echoBuffer[i] );
        }
        printf( "\n" );
        
        // Echo message back to client 
        if ( send( socket, echoBuffer, recvMsgSize, 0) != recvMsgSize ) {
            perror( "send() failed" );
            exit(1);
        }
    }
    
    return recvMsgSize;
}


int main( int argc, char *argv[] )
{
    int	serverSock;	// Socket descriptor for server 
    
    if ( argc != 2 )     
    {
        fprintf( stderr, "Usage:  %s port\n", argv[0]);
        exit(1);
    }
    
    serverSock = CreateUnixServerSocket( argv[1] );
    printf("Finished listening...\n");
    int clientSock;        				// Socket descriptor for client
    struct sockaddr_un echoClientUn; 	// client address
    unsigned int clientLen;     	// Length of client address data structure 
    
    clientSock = AcceptUnixClient( serverSock, &echoClientUn );
    
    
    while(1)
    {
        if( HandleUnixClient( clientSock ) == 0 )
        {
            printf( "Connection %d Shudown.\n", clientSock );
            close( clientSock );
            break;
        }
    }
    
    return 0;
}
