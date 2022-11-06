#include "Socket.h"


Socket::Socket(int sock)
{
    this->sock = sock;
}
char* Socket::getRequest()
{
    printf("in getRequest\n");
    const int DATA_SIZE = 5000000;
    const int BUFFER_SIZE = 1024;
    const int GET_BOUNDARY_COUNT = 1;
    const int POST_BOUNDARY_COUNT = 4;
    ssize_t rval = 0;
    WebUploadServlet http_servlet;

    char req_received[DATA_SIZE];
    char buf[BUFFER_SIZE];
    int req_pos = 0;
    int buf_pos = 0;
    while(true){
        rval = read(sock, &buf[0], BUFFER_SIZE);
//        printf("\nin while:%ld\n", rval);

        for(int i = req_pos; i < req_pos + rval; ++i){
            req_received[i] = buf[buf_pos];
            ++buf_pos;
        }
        req_pos += rval;

        buf_pos = 0;
        if(req_received[0] == 'G' && countBoundary("Accept-Language", req_received, req_pos) == GET_BOUNDARY_COUNT){
            cout << endl;
//            printf("GET read completed\n");
            break;
        }else if(req_received[0] == 'P' && countBoundary("------WebKitFormBoundary", req_received, req_pos) >= POST_BOUNDARY_COUNT){
//            printf("POST read completed\n");
            break;
        }
    }
    req_received[req_pos] = '\0';
//    for(int i = 0; i < req_pos; i++){
//        cout << req_received[i];
//    }

    if(rval < 0) perror("reading socket");

    ServletRequest req(req_received, req_pos);
    ServletResponse res;

    if(req.getMethod() == "POST"){
        printf("doPost called\n");
        http_servlet.doPost(sock, req, res);
    }
    else if(req.getMethod() == "GET") http_servlet.doGet(sock, req, res);
    else perror("undetectable request");

    return req_received;
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

int Socket::countBoundary(string boundary, char *req, ssize_t size) {
    char* temp = req;
    char* ptr = &boundary.at(0);
    int count = 0;
    int move_count = 0;
    int pos = 0;

    while(pos < size) {
        while (move_count < boundary.length() && *temp == *ptr) {
            *temp++;
            *ptr++;
            ++move_count;
        }
        if(move_count == boundary.length()) ++count;
        move_count = 0;
        ptr = &boundary.at(0);
        *temp++;
        ++pos;
    }

//    cout << "count is: " << count << endl;

    return count;
}