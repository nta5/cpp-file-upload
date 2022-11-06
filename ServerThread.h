//
// Created by Shen Yen on 2022-11-04.
//

#ifndef CPP_FILE_UPLOAD_SERVERTHREAD_H
#define CPP_FILE_UPLOAD_SERVERTHREAD_H


#include "Thread.h"
#include "Socket.h"

class ServerThread: public Thread {
private:
    Socket *socket;
    int connectionCount;
public:
    ServerThread(Socket *socket, int connectionCount);
    void run() override;
//    void start() override;
};


#endif //CPP_FILE_UPLOAD_SERVERTHREAD_H
