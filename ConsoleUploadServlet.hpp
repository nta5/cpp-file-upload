//
// Created by Li on 2022-11-02.
//

#ifndef A3_CONSOLEUPLOADSERVLET_HPP
#define A3_CONSOLEUPLOADSERVLET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define TRUE 1

using namespace std;

class ConsoleUploadServlet {
public:
    static void console();
};


#endif //A3_CONSOLEUPLOADSERVLET_HPP
