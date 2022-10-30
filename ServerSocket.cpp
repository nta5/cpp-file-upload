#include "ServerSocket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <unistd.h>
#include <iostream>

using namespace std;

ServerSocket::ServerSocket(int port)
{
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("opening stream socket");
  } else {
    cout << "opening stream socket" << endl;
  }
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8888);
  
  if (bind(sock, (struct sockaddr *)&server, sizeof server) < 0) {
    perror ("binding stream socket");
  } else {
    cout << "binding stream socket" << endl;
  }
  getsockname( sock, (struct sockaddr *) &server,(socklen_t *)sizeof server);
  printf("opened socket as fd (%d) on port (%d) for stream i/o\n",sock, ntohs(server.sin_port));

  listen(sock, 5);
}

Socket* ServerSocket::Accept()
{
	sockaddr_in localAddr, remoteAddr;
	unsigned int addrLen = sizeof (remoteAddr);
	int cSock = accept(sock, (struct sockaddr *)&remoteAddr, &addrLen);
	Socket *cs = new Socket(cSock);
	return cs;
}

ServerSocket::~ServerSocket()
{
}
