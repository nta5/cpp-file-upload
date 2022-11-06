#include "Socket.h"
#include "ServerSocket.h"
#include "ServerThread.h"
#include <stddef.h>

int main() {
    int connectionCount =  0;
    ServerSocket *ss = new ServerSocket(8888);
    if (ss != NULL) {
        while (true) {
            Socket *cs = ss->Accept();
            ServerThread *st = new ServerThread(cs, connectionCount++);
            st->start();
//            char *req = cs->getRequest();
//            char *res = new char();
//            cs->sendResponse(mRes);
        }
    }

    return 0;
}