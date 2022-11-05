//
// Created by Li on 2022-11-02.
//

#include "ConsoleUploadServlet.hpp"


#define TRUE 1

using namespace std;

void ConsoleUploadServlet::console()
{
    struct sockaddr_in addr;
    int addrlen, sock, status;
    struct ip_mreq mreq;
    char buf[50];
    static int so_reuseaddr = TRUE;
    bool loop = true;
    std::string myList[3];

    /* set up socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }


    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8888);
    addrlen = sizeof(addr);


    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &so_reuseaddr, sizeof(so_reuseaddr)) < 0) {
        perror("setsockopt reuse addr");
        exit(1);
    }

    /*bind Socket*/
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }


    mreq.imr_multiaddr.s_addr = inet_addr("230.0.0.1");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    /*Join the Multicast Group*/
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt mreq");
        exit(1);
    }

    /*receive messages destined for this multicast group*/
//    status = recvfrom(sock, buf, sizeof(buf), 0,
//                      (struct sockaddr *) &addr, &addrlen);
//    if (status < 0) {
//        perror("recvfrom");
//        exit(1);
//    }
//    printf("%s: message = \"%s\"\n", inet_ntoa(addr.sin_addr), buf);
//    if (strcmp(buf, "n") == 0) {
//        loop = false;
//    } else {
//        loop = true;
//    }

    int count = 0;
    char newBuf[1024];
    for (int i = 0; i < 3; i++) {
        status = recvfrom(sock, newBuf, sizeof(newBuf), 0,
                          (struct sockaddr *) &addr, &addrlen);
        if (status < 0) {
            perror("recvfrom");
            exit(1);
        }
        if (strcmp(newBuf, "nonono") == 0) {
            exit(1);
        }

        myList[count] = newBuf;
        count++;
        if (count == 3) {
            printf("{\"path\": %s, \"caption\": %s, \"date\": %s}", myList[0].c_str(), myList[1].c_str(), myList[2].c_str());
        }
//        printf("{\"path\": %s, \"caption\": %s, \"date\": %s}", myList[0].c_str(), myList[1].c_str(), myList[2].c_str());
        printf("%s: m1111essage = \"%s\"\n", inet_ntoa(addr.sin_addr), newBuf);
//        printf("");

    }

//    cout << "{\"path\": " << myList[0] << ", \"caption\": " << myList[1] << ", \"date\": " << myList[2] << "}";
    return;

}


