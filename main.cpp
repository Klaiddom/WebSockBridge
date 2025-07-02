#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;

class WebSocketServer {
public:
    WebSocketServer() {
        m_server.init_asio();

        m_server.set_open_handler([](connection_hdl){
            std::cout << "Client connected\n";
        });

        m_server.set_close_handler([](connection_hdl){
            std::cout << "Client disconnected\n";
        });

        m_server.set_message_handler([this](connection_hdl hdl, server::message_ptr msg){
            std::cout << "Received: " << msg->get_payload() << std::endl;
            m_server.send(hdl, msg->get_payload(), msg->get_opcode());
        });
    }

    void run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }

private:
    server m_server;
};

int main() {
    std::cout << "We are linked and compiled";
    return 0;
}
