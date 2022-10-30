#include "Socket.h"


Socket::Socket(int sock)
{
    this->sock = sock;
}
char* Socket::getRequest()
{
    int rval;
    char *res = new char[1024];
    WebUploadServlet http_servlet;

    //TODO: change to while to read through the end
    if ((rval = read(sock, res, 1024)) < 0){
        perror("reading socket");
    }else  {
        ServletRequest req(res);
        ServletResponse res;
        printf("get request\n\n");

        if(req.getMethod() == "POST") http_servlet.doPost(sock, req, res);
        else if(req.getMethod() == "GET") http_servlet.doGet(sock, req, res);
        else printf("what is this"); //for command line?

//        printf("%s\n", buf);
    }


    return nullptr;
}

void Socket::sendResponse(char *res){
    int rval;
//    res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: 513\r\n\r\n<!DOCTYPE html>\r\n<html>\n   <head>\n       <title>File Upload Form</title>\n   </head>\n   <body>\n       <h1>Upload file</h1>\n       <form method=\"POST\" action=\"upload\" enctype=\"multipart/form-data\">\n           <input type=\"file\" name=\"fileName\"/><br/><br/>\n           Caption: <input type=\"text\" name=\"caption\"<br/><br/><br/>\n           Date: <input type=\"date\" name=\"date\"<br/><br/><br/>\n           <input type=\"submit\" value=\"Submit\"/>\n       </form>\n   </body>\n</html>\r\n\r\n";

    if ((rval = write(sock, res, strlen(res))) < 0){
        perror("writing socket");
    }else  {
        printf("send response\n");
        printf("%s\n",res);
    }

    close (sock);
    return;
}
Socket::~Socket()
{
}