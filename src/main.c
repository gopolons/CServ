#include "Receiver/Receiver.h"

/*
 * This function is used for listening
 * to incoming requests and printing
 * their content to the console.
 */
int listenToIncomingRequests() {
  char port[] = "3490";

  struct addrinfo hints;
  struct addrinfo *serverInfo; // will point to results

  if (getAddressInfo(port, hints, &serverInfo) == -1) {
    exit(1);
  }

  int socketDescriptor = 0;

  if ((socketDescriptor = prepareSocket(serverInfo)) == -1) {
    exit(1);
  }

  int bufferSize = 1024;
  char *buffer = malloc(bufferSize);

  acceptRequest(socketDescriptor, buffer, bufferSize);

  // close the connection
  close(socketDescriptor);

  // free server information
  freeaddrinfo(serverInfo);

  // free the allocated buffer memory
  free(buffer);

  return 0;
}

int main(int argc, char *argv[]) { listenToIncomingRequests(); }
