#ifndef WEBSOCKBRIDGE_BRIDGE_H
#define WEBSOCKBRIDGE_BRIDGE_H
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;

class WebSocketServer {

    server m_server;

public:
    WebSocketServer();
    void run(uint16_t port);
};
#endif
