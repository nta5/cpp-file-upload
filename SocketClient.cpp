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

void parseResponse(char* res);

int main()
{
    struct sockaddr_in addr;
    int addrlen, sock, status, client_fd;
    struct ip_mreq mreq;
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

    //test on your own: IP address should be your wifi ip address
    //test with others: server's IP address
    addr.sin_addr.s_addr = inet_addr("192.168.10.247");
    /*addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
    addr.sin_port = htons(8888);
    addrlen = sizeof(addr);

    if ((client_fd = connect(sock, (struct sockaddr*)&addr,
                           sizeof(addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    cout << "Would you like to upload a image? (y/n)"<<endl;
    string checking;
    cin >> checking;
    if(checking != "Y" && checking != "y"){
        cout << "Terminating the program." << endl;
        exit(1);
    }

    cout << "Enter the path:" << endl;
    string pathInput;
    cin >> pathInput;
    //parse the file name from the path and save into string
    int begPos = pathInput.find_last_of('/');
    string fileName = pathInput.substr(begPos + 1, pathInput.size() - begPos);

    cout << "Enter the caption:" << endl;
    string captionInput;
    cin >> captionInput;

    cout << "Enter the date:" << endl;
    string dateInput;
    cin >> dateInput;

    ifstream ifd(pathInput, ios::binary | ios::ate);
    int size = ifd.tellg();
    ifd.seekg(0, ios::beg);
    vector<char> buffer;
    buffer.resize(size); // << resize not reserve
    ifd.read(buffer.data(), size);

    int fileSize = buffer.size();
    unsigned char bytesArray[fileSize];
    for(int i = 0; i < fileSize; i++){
        bytesArray[i] = buffer.at(i);
//        printf("%lx ", bytesArray[i]);
    }
//    printf("\n");

    //<--------------------- HEADER PART ----------------------------->
    string header = "POST /upload HTTP/1.1\r\nHost: console\r\nConnection: keep-alive\r\n";
    header += "Cache-Control: max-age=0\\r\\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://localhost:8888\r\nContent-Type: multipart/form-data; boundary=-----WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\"\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nReferer: http://localhost:8888/\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: en-US,en;q=0.9\r\n";
    int headerSize = header.size();
//    cout << "header size:" << headerSize << endl;

    const char* headerArr = header.c_str();

    //<--------------------- FILE PART ----------------------------->
    string filePart = "\r\n------WebKitFormBoundaryC0eh71BbS7kAj3Wn\r\n";
    filePart += "Content-Disposition: form-data; name=\"fileName\"; filename=\"" + fileName+ "\"\r\n";
    filePart += "Content-Type: image/png\r\n\r\n";
    int filePartSize = filePart.size();
//    cout << "filePart size:" << filePartSize << endl;

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
//    cout << "fileInfo size:" << fileInfoSize << endl;

    const char *fileInfoArr = fileInfo.c_str();

    //<--------------------- CREATE REQUEST----------------------------->
    int reqSize = headerSize + filePartSize + fileSize + fileInfoSize;
//    cout << "request size:" << reqSize << endl;

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

//    cout << "pos is:" << pos << endl;
//    for(int i = 0; i < reqSize; ++i){
//        cout << req[i];
//    }
//    cout << endl;


    int rval;
    int readVal;
    char res[10000];

    if ((rval = write(sock, req, reqSize)) < 0){
        perror("writing socket");
    }else  {
        shutdown(sock, SHUT_WR);
        readVal = read(sock, res, 10000);
//        cout << "pass rval:" << rval << endl;
//        cout << "read from socket:" << readVal << endl;
        parseResponse(res);
    }
}

void parseResponse(char* res){
    const string JASON_DELIMITER = "<JSON>";
    string response = res;
//    cout << response << endl;
    vector<string> lines;

    int pos = response.find(JASON_DELIMITER);
    string json = response.substr(pos + JASON_DELIMITER.length(), response.length() - pos);
    cout << json << endl;
}
