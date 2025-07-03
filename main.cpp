#include "bridge/headers/bridge.h"


int main() {
    WebSocketServer server;
    server.run(8001);
    return 0;
}
