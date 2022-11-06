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
#include <vector>
#include <fstream>

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

    ifstream ifd("C:\\Users\\redst\\Pictures\\Screenshot 2021-09-10 171216.png", ios::binary | ios::ate);
    int size = ifd.tellg();
    ifd.seekg(0, ios::beg);
    vector<char> buffer;
    buffer.resize(size); // << resize not reserve
    ifd.read(buffer.data(), size);

    //cout.write(buffer.data(), buffer.size());
    string bytes(buffer.begin(),buffer.end());

    string payload = "";
    payload += "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    payload += "Content-Disposition: form-data; name=\"fileName\"; fileName=\"file.png\"\r\n";
    payload += "Content-Type: image/png\r\n\r\n";
    payload += bytes;
    payload += "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    payload += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
    payload += "hi\r\n";
    payload += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    payload += "Content-Disposition: form-data; name=\"date\"\r\n\r\n";
    payload += "2022\r\n";
    payload += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn--\r\n\r\n";

    int rval;
    string hdr = "POST /upload HTTP/1.1\r\nHost: localhost:8888\r\nConnection: keep-alive\r\nContent-Length: ";
    hdr += to_string(payload.length());
    hdr += "\r\nCache-Control: max-age=0\\r\\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://localhost:8888\r\nContent-Type: multipart/form-data; boundary=------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\"\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nReferer: http://localhost:8888/\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9\r\n";
    cout << hdr.length() << endl;
    hdr += payload;
    char res[hdr.length() + 1];
    strcpy(res, hdr.c_str());

    if ((rval = write(sock, res, strlen(res))) < 0){
        perror("writing socket");
    }else  {
        cout << "pass";
    }

//    char buffer[1024] = { 0 };
//    rval = read(sock, buffer, 1024);
//    printf("%s\n", buffer);

    close(sock);
}
