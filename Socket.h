#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include "WebUploadServlet.hpp"

using namespace std;

class Socket
{
public:
    Socket(int sock);
    char* getRequest();
    void sendResponse(char* res);
    ~Socket();
private:
    int sock;
};
