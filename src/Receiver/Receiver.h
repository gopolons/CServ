#ifndef Receiver_h
#define Receiver_h

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG_SIZE 5

/*
 * A function for accepting an incoming request.
 * Takes in a char buffer pointer argument,
 * & buffer size which will be used for
 * writing the received data to and the file
 * descriptor for the socket.
 *
 * Returns the number of received bytes
 * on success & -1 on failure.
 */
int acceptRequest(int socketDescriptor, char *buffer, int bufferSize);

/*
 * A function for requesting address information
 * for that will be used for requesting a socket
 * from the system and binding it to a port.
 *
 * Takes a port argument, which is used for
 * creating host address information.
 *
 * Returns 0 on success & -1 on failure.
 */
int getAddressInfo(char port[5], struct addrinfo hints,
                   struct addrinfo **serverInfo);

/*
 * A function which takes address information and
 * a port argument and binds a socket using the
 * data on the specified port. Upon activating,
 * will start listening for incoming connections.
 *
 * Returns a socket descriptor or a -1 on failure.
 */
int prepareSocket(struct addrinfo *serverInfo);

#endif // !Receiver_h
