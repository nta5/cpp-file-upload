#include "Socket.h"


Socket::Socket(int sock)
{
    this->sock = sock;
}
char* Socket::getRequest()
{
    int rval;
    char *buf = new char[1024];
    WebUploadServlet http_servlet;
    ServletRequest req;
    ServletResponse res;

    if ((rval = read(sock, buf, 1024)) < 0){
        perror("reading socket");
    }else  {
        printf("get request\n\n");

        stringstream ss(buf);
        string type;
        if(ss >> type){
            if(type == "POST") http_servlet.doPost(req, res);
            else if(type == "GET") http_servlet.doGet(req, res);
            else printf("what is this");
        }else {
            cout << "parse fail" << endl;
        }

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
        printf("send response\n");
        printf("%s\n",res);
    }

    close (sock);
    return;
}
Socket::~Socket()
{
}