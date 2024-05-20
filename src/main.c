#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * This function is used for listening
 * to incoming requests and printing
 * their content to the console.
 */
int acceptIncomingRequest() {
  char port[] = "3490";

  // get address information
  int addressStatus = 0;
  struct addrinfo hints;
  struct addrinfo *serverInfo; // will point to results

  memset(&hints, 0, sizeof hints); // ensure struct is empty

  hints.ai_family = AF_UNSPEC;     // either IPv4 or IPv6 works
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // get IP automatically

  addressStatus = getaddrinfo(NULL, port, &hints, &serverInfo);

  // check for error
  if (addressStatus == -1) {
    printf("[ERROR] Failed to acquire address info: %s\n", errno);
    exit(1);
  }

  // check server info: if empty, assign the next value in the linked list
  while (!(serverInfo->ai_family) || !(serverInfo->ai_family) ||
         !(serverInfo->ai_family)) {
    serverInfo = serverInfo->ai_next;
  }

  // acquire socket descriptor
  int socketDescriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                                serverInfo->ai_protocol);

  // check for error
  if (socketDescriptor == -1) {
    printf("[ERROR] Failed to acquire a socket descriptor: %s\n", errno);
    exit(1);
  }

  // bind to a port
  int bindStatus = 0;

  bindStatus =
      bind(socketDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen);

  // check for error
  if (bindStatus == -1) {
    printf("[ERROR] Failed to bind socket on port %s: %s\n", port, errno);
    exit(1);
  }

  // allow the socket to be reused
  int yes = 1;

  bindStatus =
      setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  // check for error
  if (bindStatus == -1) {
    printf("[ERROR] Failed to enable reuse policy for socket on port %s\n",
           port);
    exit(1);
  }

  // listen for incoming connections
  int listenStatus = 0;

  printf("[LOG] Listening on socket on port %s\n", port);

  listen(socketDescriptor, 5);

  // check for error
  if (listenStatus == -1) {
    printf("[ERROR] Failed to start listening on socket on port %s: %s\n", port,
           errno);
    exit(1);
  }

  // accept an incoming connection
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  addr_size = sizeof(their_addr);
  int newSocketDescriptor =
      accept(socketDescriptor, (struct sockaddr *)&their_addr, &addr_size);

  // receive the data
  int bufferSize = 1024;
  char *buffer = malloc(bufferSize);
  int receivedBytes = recv(newSocketDescriptor, buffer, bufferSize - 1, 0);

  // check for error
  if (receivedBytes == -1) {
    printf("[ERROR] Failed to receive data on socket running on port %s: %s\n",
           port, errno);
    exit(1);
  }

  buffer[receivedBytes] = '\0'; // null-terminate received data

  printf("[LOG] Received data on the socket %s: %s\n", port, buffer);

  // close the connection
  close(newSocketDescriptor);
  close(socketDescriptor);

  // free server information
  freeaddrinfo(serverInfo);

  return 0;
}

int main(int argc, char *argv[]) { acceptIncomingRequest(); }
