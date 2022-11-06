//
// Created by Shen Yen on 2022-11-04.
//

#include "ServerThread.h"

ServerThread::ServerThread(Socket *socket, int connectionCount) {
    this->socket = socket;
    this->connectionCount = connectionCount;
}

void ServerThread::run() {
    char *req = socket->getRequest();
    char *res = new char();
//            cs->sendResponse(mRes);
}
