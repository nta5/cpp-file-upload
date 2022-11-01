#include "Socket.h"


Socket::Socket(int sock)
{
    this->sock = sock;
}
char* Socket::getRequest()
{
    const int HEADER_SIZE = 874;
    int rval;
    char *header = new char[HEADER_SIZE];
    WebUploadServlet http_servlet;

    if ((rval = read(sock, header, HEADER_SIZE)) < 0){
        perror("reading socket (header)");
    }else  {
        ServletRequest req(header);
        ServletResponse res;

        if(req.getMethod() == "POST"){
            const int BODY_SIZE = req.getContentLength();
            char *body = new char[BODY_SIZE];

            rval = read(sock, body, BODY_SIZE);
            while(rval < BODY_SIZE && rval > 0){
//                printf("READ ING ING ING, rval : %d\n", rval);
                char* temp = new char[BODY_SIZE];
                int size = read(sock, temp, BODY_SIZE);
                int pos = 0;
                for(int i = rval; i < rval + size; ++i){
                    body[i] = temp[pos];
                    ++pos;
                }
                rval += size;
                delete[] temp;
            }

            if(rval < 0){
                perror("reading socket (body)");
            }else{
//                printf("READ FROM SOCKET: %d\n", rval);
                req.setBody(body);
                req.parseFilePart();
            }
            http_servlet.doPost(sock, req, res);
        }
        else if(req.getMethod() == "GET"){
            http_servlet.doGet(sock, req, res);
        }
        else{
            printf("what is this"); //for command line?
        }
    }


    return nullptr;
}

void Socket::sendResponse(char *res){
    int rval;
//    res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: 513\r\n\r\n<!DOCTYPE html>\r\n<html>\n   <head>\n       <title>File Upload Form</title>\n   </head>\n   <body>\n       <h1>Upload file</h1>\n       <form method=\"POST\" action=\"upload\" enctype=\"multipart/form-data\">\n           <input type=\"file\" name=\"fileName\"/><br/><br/>\n           Caption: <input type=\"text\" name=\"caption\"<br/><br/><br/>\n           Date: <input type=\"date\" name=\"date\"<br/><br/><br/>\n           <input type=\"submit\" value=\"Submit\"/>\n       </form>\n   </body>\n</html>\r\n\r\n";

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