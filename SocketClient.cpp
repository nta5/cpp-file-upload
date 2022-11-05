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
    int addrlen, sock, status;
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
    /*addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
    addr.sin_port = htons(8888);
    addrlen = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("230.0.0.1");

    cout << "Do u want to upload a image?(y/n)" << endl;
    char check[1024];
    cin >> check;
    if (strcmp(check, "y") == 0) {
        loop = true;
    } else {
        loop = false;
    }

    while (loop) {
        cout << "Enter the path" << endl;
        char path1[1024];
        cin >> path1;
        strcpy(path, path1);
        status = sendto(sock, path, sizeof(path), 0,
                        (struct sockaddr *) &addr, addrlen);
        if (status < 0) {
            perror("sendto");
            exit(1);
        }

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

        cout << "Enter the caption" << endl;
        char x[1024];
        cin >> x;
        strcpy(caption, x);
        status = sendto(sock, caption, sizeof(caption), 0,
                        (struct sockaddr *) &addr, addrlen);
        if (status < 0) {
            perror("sendto");
            exit(1);
        }

        cout << "Enter the date" << endl;
        char y[1024];
        cin >> y;
        strcpy(date, y);
        status = sendto(sock, date, sizeof(date), 0,
                        (struct sockaddr *) &addr, addrlen);
        if (status < 0) {
            perror("sendto");
            exit(1);
        }

        cout << "Successful upload a image." << endl;


//        cout << path << " " << caption << " " << date;
        loop = false;
//        cout << "Do you want to upload another image?(y/n)" << endl;
//        char a[1024];
//        cin >> a;
//        if (strcmp(a, "y") != 0) {
//            loop = false;
//        }

    }

}
