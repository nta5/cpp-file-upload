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
    addr.sin_addr.s_addr = inet_addr("172.20.10.5");
    /*addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
    addr.sin_port = htons(8888);
    addrlen = sizeof(addr);

    if ((client_fd = connect(sock, (struct sockaddr*)&addr,
                           sizeof(addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    cout << "Would you like to upload a image??(y/n)"<<endl;
    char checking[1024];
    cin >> checking;
//    if(strcmp(checking,'y') == 0){
//        loop = true;
//    } else{
//        loop = false;
//    }
//    while(loop){
//        cout << "Enter the path" << endl;
//        char path[1024];
//        cin >> path;
//    }
    cout << "Enter the path" << endl;
//    char pathInput[1024];
    string pathInput;
    cin >> pathInput;
    //parse the file name from the path and save into string


    cout << "Enter the caption" << endl;
//    char captionInput[1024];
    string captionInput;
    cin >> captionInput;

    cout << "Enter the date" << endl;
//    char dateInput[1024];
    string dateInput;
    cin >> dateInput;

    ifstream ifd(pathInput, ios::binary | ios::ate);
    int size = ifd.tellg();
    ifd.seekg(0, ios::beg);
    vector<char> buffer;
    buffer.resize(size); // << resize not reserve
    ifd.read(buffer.data(), size);

    //cout.write(buffer.data(), buffer.size());
//    string bytes(buffer.begin(),buffer.end());
    int fileSize = buffer.size();
    unsigned char bytesArray[fileSize];
    for(int i = 0; i < fileSize; i++){
        bytesArray[i] = buffer.at(i);
//        printf("%lx ", bytesArray[i]);
//        cout << bytesArray[i];
    }
//    printf("\n");
//    cout << endl;

    //<--------------------- HEADER PART ----------------------------->
    string header = "POST /upload HTTP/1.1\r\nHost: localhost:8888\r\nConnection: keep-alive\r\n";
    header += "Cache-Control: max-age=0\\r\\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://localhost:8888\r\nContent-Type: multipart/form-data; boundary=-----WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\"\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nReferer: http://localhost:8888/\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9\r\n";
    int headerSize = header.size();
    cout << "header size:" << headerSize << endl;

    const char* headerArr = header.c_str();

    //<--------------------- FILE PART ----------------------------->
    string filePart = "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    filePart += "Content-Disposition: form-data; name=\"fileName\"; filename=\"" +captionInput+".png\"\r\n";
    filePart += "Content-Type: image/png\r\n\r\n";
    int filePartSize = filePart.size();
    cout << "filePart size:" << filePartSize << endl;

    const char *filePartArr = filePart.c_str();

    //<--------------------- FILE INFO ----------------------------->
    string fileInfo = "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    fileInfo += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
    fileInfo += captionInput + "\r\n";
    fileInfo += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    fileInfo += "Content-Disposition: form-data; name=\"date\"\r\n\r\n";
    fileInfo +=  dateInput + "\r\n";
    fileInfo += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn--\r\n\r\n";
    int fileInfoSize = fileInfo.size();
    cout << "fileInfo size:" << fileInfoSize << endl;

    const char *fileInfoArr = fileInfo.c_str();

    //<--------------------- CREATE REQUEST----------------------------->
    int reqSize = headerSize + filePartSize + fileSize + fileInfoSize;
    cout << "request size:" << reqSize << endl;

    char* req = new char [reqSize];

    int pos = 0;
    for(int i = 0; i < headerSize; ++i){
        req[pos] = headerArr[i];
        ++pos;
    }
    for(int i = 0; i < filePartSize; ++i){
        req[pos] = filePartArr[i];
        ++pos;
    }
    for(int i = 0; i < fileSize; ++i){
        req[pos] = bytesArray[i];
        ++pos;
    }
    for(int i = 0; i < fileInfoSize; ++i){
        req[pos] = fileInfoArr[i];
        ++pos;
    }

    cout << "pos is:" << pos << endl;
    for(int i = 0; i < reqSize; ++i){
        cout << req[i];
    }
    cout << endl;

//    string payload = "";
//    payload += "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
//    payload += "Content-Disposition: form-data; name=\"fileName\"; fileName=\"file.png\"\r\n";
//    payload += "Content-Type: image/png\r\n\r\n";
//    payload += bytes;
//    payload += "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
//    payload += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
//    payload += "hi\r\n";
//    payload += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
//    payload += "Content-Disposition: form-data; name=\"date\"\r\n\r\n";
//    payload += "2022\r\n";
//    payload += "------WebKitFormBoundaryC0eh71BbS7kAj3Wn--\r\n\r\n";

    int rval;
//    string hdr = "POST /upload HTTP/1.1\r\nHost: localhost:8888\r\nConnection: keep-alive\r\nContent-Length: ";
//    hdr += to_string(payload.length());
//    hdr += "\r\nCache-Control: max-age=0\\r\\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://localhost:8888\r\nContent-Type: multipart/form-data; boundary=------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\"\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nReferer: http://localhost:8888/\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9\r\n";
//    cout << hdr.length() << endl;
//    hdr += payload;
//    char res[hdr.length() + 1];
//    strcpy(res, hdr.c_str());

    if ((rval = write(sock, req, reqSize)) < 0){
        perror("writing socket");
    }else  {
        shutdown(sock, SHUT_WR);
        cout << "pass rval:" << rval << endl;
    }

//    char buffer[1024] = { 0 };
//    rval = read(sock, buffer, 1024);
//    printf("%s\n", buffer);

//    close(sock);
}
