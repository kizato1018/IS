#include <stdio.h>          // for printf() and fprintf() 
#include <sys/socket.h>     // for socket(), bind(), and connect() 
#include <arpa/inet.h>      // for sockaddr_in and inet_ntoa() 
#include <stdlib.h>         // for atoi() and exit() 
#include <string.h>         // for memset() 
#include <unistd.h>         // for close() 
#include <sys/time.h>       // for struct timeval {} 
#include <fcntl.h>          // for fcntl() 

#define RCVBUFSIZE 1000	    // Buffer Size

int CreateUDPServerSocket(unsigned short port)
{
    int sock;                        // socket to create 
    struct sockaddr_in echoServerAddr; // Local address 

    // Create socket for incoming connections 
    if ( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        perror( "socket() failed" );
        exit(1);
    }
      
    // Construct local address structure 
    memset( &echoServerAddr, 0, sizeof( echoServerAddr ) );	// Zero out structure 
    echoServerAddr.sin_family = AF_INET;                      	// Internet address family 
    echoServerAddr.sin_addr.s_addr = inet_addr( "0.0.0.0" );   // Any incoming interface 
    // echoServerAddr.sin_addr.s_addr = htonl( INADDR_ANY );
                                                                // # define INADDR_ANY ((unsigned long int) 0x00000000)
    echoServerAddr.sin_port = htons( port );                  	// Local port 

    // Bind to the local address 
    if ( bind(sock, (struct sockaddr *) &echoServerAddr, sizeof( echoServerAddr ) ) < 0 )
    {
        perror( "bind() failed" );
        exit(1);
    }
    
    return sock;
}

int HandleUDPClient( int socket )
{
    char    echoBuffer[RCVBUFSIZE] = {0};  // Buffer for echo string 
    int     recvMsgSize;                   // Size of received message 
    struct sockaddr_in echoClientAddr;	   // Client Address 
    int len = sizeof( echoClientAddr );;
    
    // Receive message from client 
    if ( ( recvMsgSize = recvfrom( socket, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClientAddr, &len ) ) < 0 )
    {
        perror("recvfrom() failed");
        exit(1);
    }

    if ( recvMsgSize > 0 ) 
    {
        printf( "From FD (%d), Address(%s:%d),  recv %d bytes:\n\tData:", 
                socket,
                inet_ntoa( echoClientAddr.sin_addr ), 
                ntohs( echoClientAddr.sin_port ) , 
                recvMsgSize );
        
        for( int i = 0 ; i < recvMsgSize ; i++ )
        {
            printf( "%02X ", echoBuffer[i] );
        }
        printf( "\n" );
        
        // Echo message back to client 
        if ( sendto( socket, echoBuffer, recvMsgSize, 0, (const struct sockaddr *) &echoClientAddr, sizeof( echoClientAddr ) ) != recvMsgSize )
        {
            perror( "sendto() failed" );
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
    
    serverSock = CreateUDPServerSocket( atoi( argv[1] ) );
    
    while(1)
    {
        HandleUDPClient( serverSock );
    }
    
    return 0;
}
