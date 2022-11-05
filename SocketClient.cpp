#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    struct sockaddr_in addr;
    int addrlen, sock, status, client_fd;
    struct ip_mreq mreq;
    char path[500];
    char path2[500];
    char caption[500];
    char date[500];
    int loop;
    time_t t;

    /* set up socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.75");
    /*addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
    addr.sin_port = htons(8888);
    addrlen = sizeof(addr);

    if ((client_fd = connect(sock, (struct sockaddr*)&addr,
                           sizeof(addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    cout << client_fd;

    string payload = "";
    payload += "\r\n----WebKitFormBoundary4ww5j9J1LNOFEB9f\r\n";
    payload += "Content-Disposition: form-data; name=\"filename\"\r\n\r\n";
    payload += "file.jpg\r\n";
    payload += "----WebKitFormBoundary4ww5j9J1LNOFEB9f\r\n";
    payload += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
    payload += "hi\r\n";
    payload += "----WebKitFormBoundary4ww5j9J1LNOFEB9f\r\n";
    payload += "Content-Disposition: form-data; name=\"date\"\r\n\r\n";
    payload += "2022\r\n";
    payload += "----WebKitFormBoundary4ww5j9J1LNOFEB9f--\r\n\r\n";

    int rval;
    string hdr = "";
    hdr += "POST /upload HTTP/1.1\r\n";
    hdr += "Host: localhost:8888\r\n";
    hdr += "Connection: keep-alive\r\n";
    hdr += "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary4ww5j9J1LNOFEB9f\\r\\n";
    hdr += "Content-Length:" + to_string(payload.size()) + "\r\n";
    hdr += "\r\n";
    hdr += payload;
    char res[hdr.length() + 1];
    strcpy(res, hdr.c_str());

    if ((rval = write(sock, res, strlen(res))) < 0){
        perror("writing socket");
    }else  {
        cout << "pass";
    }

}
