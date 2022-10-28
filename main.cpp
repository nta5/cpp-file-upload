#include "Socket.h"
#include "ServerSocket.h"
#include <stddef.h>
int main() {
	ServerSocket *ss = new ServerSocket(8888);
	if (ss != NULL) {
		while (true) {
			Socket *cs = ss->Accept();
			char *req = cs->getRequest();
			char *res = new char(50);
			cs->sendResponse(res);
		}
	}

    return 0;
}
