#include <stdio.h>          // for printf() and fprintf() 
#include <sys/socket.h>     // for socket(), bind(), and connect() 
#include <arpa/inet.h>      // for sockaddr_in and inet_ntoa() 
#include <stdlib.h>         // for atoi() and exit() 
#include <string.h>         // for memset() 
#include <unistd.h>         // for close() 
#include <sys/time.h>       // for struct timeval {} 
#include <fcntl.h>          // for fcntl() 
#include <sys/types.h>
#include <unistd.h>

#define MAXPENDING 5        // Maximum outstanding connection requests
#define RCVBUFSIZE 1000	    // Buffer Size

int CreateTCPServerSocket(unsigned short port)
{
    int sock;                        // socket to create 
    struct sockaddr_in echoServerAddr; // Local address 

    // Create socket for incoming connections 
    if ( ( sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
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
    
    // Mark the socket so it will listen for incoming connections 
    if ( listen( sock, MAXPENDING ) < 0 )
    {
        perror( "listen() failed" );
        exit(1);
    }
    
    return sock;
}

int AcceptTCPClient( int serverSock, struct sockaddr_in *echoClientAddr )
{
	int clientSock;        				// Socket descriptor for client
    unsigned int        clientLen;     	// Length of client address data structure 
    
    // Wait for a client to connect 
    if ( ( clientSock = accept( serverSock, (struct sockaddr *) echoClientAddr, &clientLen ) ) < 0 )
    {
        perror("accept() failed");
        exit(1);
    }

    return clientSock;
}

int HandleTCPClient( int socket )
{
    char    echoBuffer[RCVBUFSIZE] = {0};  // Buffer for echo string 
    int     recvMsgSize;                   // Size of received message 
    
    // Receive message from client 
    if ( ( recvMsgSize = recv( socket, echoBuffer, RCVBUFSIZE, 0 ) ) < 0 )
    {
        perror("recv() failed");
        exit(1);
    }

    // Send received string and receive again until end of transmission 
    if ( recvMsgSize > 0 )      // zero indicates end of transmission 
    {
        printf( "From FD (%d) recv %d bytes:\n\tData:", socket, recvMsgSize );
        
        for( int i = 0 ; i < recvMsgSize ; i++ )
        {
            printf( "%02X ", echoBuffer[i] );
        }
        printf( "\n" );
        
        // Echo message back to client 
        if ( send( socket, echoBuffer, recvMsgSize, 0) != recvMsgSize )
        {
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
    
    serverSock = CreateTCPServerSocket( atoi( argv[1] ) );
    
    // FD_SET
    fd_set activeFdSet;
    FD_ZERO( &activeFdSet );
    FD_SET( serverSock, &activeFdSet );
    int maxFd = serverSock;
        
    while(1)
    {
        int             ret;
        struct timeval  tv;
        fd_set          readFds;
 
        /* Set timeout */
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        
        readFds = activeFdSet;
        ret = select( maxFd + 1, &readFds, NULL, NULL, &tv );
        
        if( ret == -1 )
        {
            // Error
            perror( "select()" );
            return -1;
        }
        else if( ret == 0 )
        {
            // Timeout
            printf( "Timeout\n" );
            continue;
        }
        else
        {
            for( int i = 0 ; i < FD_SETSIZE; i++ )
            {
                if( FD_ISSET( i, &readFds ) )
                {
                    if( i == serverSock )
                    {
                        // Accept new connections
                        int clientSock; 			// Socket descriptor for client
                        struct sockaddr_in echoClientAddr; 	// client address
                        unsigned int        clientLen;     	// Length of client address data structure 
    
                        clientSock = AcceptTCPClient( serverSock, &echoClientAddr );
                        printf("Connected Client --> IP: %s, Port: %d, FD: %d\n", inet_ntoa( echoClientAddr.sin_addr ), htons( echoClientAddr.sin_port ), clientSock );
                        
                        FD_SET( clientSock, &activeFdSet );
                        
                        if( clientSock > maxFd )
                        {
                            maxFd = clientSock;
                        }
                    }
                    else
                    {
                        // Handle Clients' Data
                        if( HandleTCPClient( i ) == 0 )
                        {
                            printf( "Connection %d Shudown.\n", i );
                            close( i );
                            FD_CLR( i, &activeFdSet );
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}
