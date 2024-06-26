#include "Receiver.h"
#include "globals.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int acceptRequest(int socketDescriptor, char *buffer, int bufferSize) {

  int listenStatus = 0;

  printf("[LOG] Listening to incoming connections\n");

  listen(socketDescriptor, BACKLOG_SIZE);

  // check for error
  if (listenStatus == -1) {
    printf("[ERROR] Failed to start listening to incoming connections: %s\n",
           strerror(errno));
    return listenStatus;
  }

  // accept an incoming connection
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  addr_size = sizeof(their_addr);
  int newSocketDescriptor =
      accept(socketDescriptor, (struct sockaddr *)&their_addr, &addr_size);

  // receive the data
  int receivedBytes = recv(newSocketDescriptor, buffer, bufferSize - 1, 0);

  // check for error
  if (receivedBytes == -1) {
    printf("[ERROR] Failed to receive data: %s\n", strerror(errno));
    return receivedBytes;
  }

  buffer[receivedBytes] = '\0'; // null-terminate received data

  printf("[LOG] Received data: %s\n", buffer);

  // close the connection
  close(newSocketDescriptor);

  return receivedBytes;
}

int getAddressInfo(struct addrinfo hints, struct addrinfo **serverInfo) {

  int addressStatus = 0;

  memset(&hints, 0, sizeof hints); // ensure struct is empty

  hints.ai_family = AF_UNSPEC;     // either IPv4 or IPv6 works
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // get IP automatically

  addressStatus = getaddrinfo(NULL, port, &hints, serverInfo);

  // check for error
  if (addressStatus == -1) {
    printf("[ERROR] Failed to acquire address info: %s\n", strerror(errno));
    return addressStatus;
  }

  // check server info: if empty, assign the next value in the linked list
  while (!((*serverInfo)->ai_family) || !((*serverInfo)->ai_family) ||
         !((*serverInfo)->ai_family)) {
    *serverInfo = (*serverInfo)->ai_next;
  }

  printf("[LOG] Acquired host address details\n");

  return addressStatus;
}

int prepareSocket(struct addrinfo *serverInfo) {

  int socketDescriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                                serverInfo->ai_protocol);

  // check for error
  if (socketDescriptor == -1) {
    printf("[ERROR] Failed to acquire a socket descriptor: %s\n",
           strerror(errno));
    return socketDescriptor;
  }

  // bind to a port
  int bindStatus = 0;

  bindStatus =
      bind(socketDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen);

  // check for error
  if (bindStatus == -1) {
    printf("[ERROR] Failed to bind socket: %s\n", strerror(errno));
    return bindStatus;
  }

  // allow the socket to be reused
  int yes = 1;

  bindStatus =
      setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  // check for error
  if (bindStatus == -1) {
    printf("[ERROR] Failed to enable reuse policy for socket %s\n",
           strerror(errno));
    return bindStatus;
  }

  printf("[LOG] Starting socket on port %s\n", port);

  return socketDescriptor;
}
