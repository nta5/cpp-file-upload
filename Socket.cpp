#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

Socket::Socket(int sock)
{
	this->sock = sock;
}
char* Socket::getRequest()
{
  int rval;
  char *buf = new char[1024];

  if ((rval = read(sock, buf, 1024)) < 0){
    perror("reading socket");
  }else  {
    printf("%s\n",buf);
  }

	return buf;
}
void Socket::sendResponse(char *res){
  int rval;
  res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: 513\r\n\r\n<!DOCTYPE html>\r\n<html>\n   <head>\n       <title>File Upload Form</title>\n   </head>\n   <body>\n       <h1>Upload file</h1>\n       <form method=\"POST\" action=\"upload\" enctype=\"multipart/form-data\">\n           <input type=\"file\" name=\"fileName\"/><br/><br/>\n           Caption: <input type=\"text\" name=\"caption\"<br/><br/><br/>\n           Date: <input type=\"date\" name=\"date\"<br/><br/><br/>\n           <input type=\"submit\" value=\"Submit\"/>\n       </form>\n   </body>\n</html>\r\n\r\n";

  if ((rval = write(sock, res, strlen(res))) < 0){
    perror("writing socket");
  }else  {
    printf("%s\n",res);
  }

  close (sock);
	return;
}
Socket::~Socket()
{
}
