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
    sock = socket(AF_INET, SOCK_DGRAM, 0);
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

//    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)
//        <= 0) {
//        printf(
//                "\nInvalid address/ Address not supported \n");
//        return -1;
//    }

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
//
//    char buffer[1024] = { 0 };
//    int valread = read(sock, buffer, 1024);
//    printf("%s\n", buffer);

//    while (loop) {
//        cout << "Enter the path" << endl;
//        char path1[1024];
//        cin >> path1;
//        strcpy(path, path1);
//        status = sendto(sock, path, sizeof(path), 0,
//                        (struct sockaddr *) &addr, addrlen);
//        if (status < 0) {
//            perror("sendto");
//            exit(1);
//        }
//
//        cout << "Confirm the path" << endl;
//        char confirmPath[1024];
//        cin >> confirmPath;
//        strcpy(path2, confirmPath);
//        if (strcmp(path, path2) != 0) {
//            cout << "Please enter same path!" << endl;
//            char stop[500]  = "nonono";
//            sendto(sock, stop, sizeof(stop), 0,(struct sockaddr *) &addr, addrlen);
//            exit(1);
//        }
//
//        status = sendto(sock, path2, sizeof(path2), 0,
//                        (struct sockaddr *) &addr, addrlen);
//        if (status < 0) {
//            perror("sendto");
//            exit(1);
//        }
//
//        cout << "Enter the caption" << endl;
//        char x[1024];
//        cin >> x;
//        strcpy(caption, x);
//        status = sendto(sock, caption, sizeof(caption), 0,
//                        (struct sockaddr *) &addr, addrlen);
//        if (status < 0) {
//            perror("sendto");
//            exit(1);
//        }
//
//        cout << "Enter the date" << endl;
//        char y[1024];
//        cin >> y;
//        strcpy(date, y);
//        status = sendto(sock, date, sizeof(date), 0,
//                        (struct sockaddr *) &addr, addrlen);
//        if (status < 0) {
//            perror("sendto");
//            exit(1);
//        }
//
//        cout << "Successful upload a image." << endl;
//
//
//        cout << path << " " << caption << " " << date;
//        loop = false;
//        cout << "Do you want to upload another image?(y/n)" << endl;
//        char a[1024];
//        cin >> a;
//        if (strcmp(a, "y") != 0) {
//            loop = false;
//        }
//
//    }

}
