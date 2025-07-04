#ifndef WEBSOCKBRIDGE_BRIDGE_H
#define WEBSOCKBRIDGE_BRIDGE_H
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <fstream>

#include "../../external/simpleserver/client/headers/http_client.h"
#include "safeguards.h"
#include "user_aggregator.h"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;

class WebSocketServer {

    server m_server;
    http::TCPClient* client;
    WSUserManager* user_manager;
    std::thread* message_handler_thread;

    void connectToServer(int server_port);
    void handle_messages();

public:
    WebSocketServer();
    void run(uint16_t port);
};
#endif
