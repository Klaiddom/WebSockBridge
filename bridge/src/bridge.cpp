#include "../headers/bridge.h"

WebSocketServer::WebSocketServer() {
    m_server.init_asio();

    m_server.set_open_handler([](connection_hdl hdl){std::cout << "Client connected\n";});

    m_server.set_close_handler([](connection_hdl hdl){std::cout << "Client disconnected\n";});

    m_server.set_message_handler([this](connection_hdl hdl, const server::message_ptr& msg){
        std::cout << "Received: " << msg->get_payload() << std::endl;
        m_server.send(hdl, msg->get_payload(), msg->get_opcode());
    });

    m_server.set_http_handler([this](connection_hdl hdl) {
        auto con = m_server.get_con_from_hdl(hdl);
        std::string path = con->get_resource();
        std::string filename = "../web/";

        if (path == "/" || path == "/index.html") {
            filename += "index.html";
        } else if (path == "/chat.html") {
            filename += "chat.html";
        } else if(path == "/app.js"){
            filename += "app.js";
        }

        if (filename.size() > 7) {
            std::ifstream file(filename);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                con->set_body(buffer.str());
                con->set_status(websocketpp::http::status_code::ok);
                con->replace_header("Content-Type", "text/html");
            } else {
                con->set_body("Source is not found");
                con->set_status(websocketpp::http::status_code::internal_server_error);
            }
        } else {
            con->set_body("404 Not Found");
            con->set_status(websocketpp::http::status_code::not_found);
        }
    });
}

void WebSocketServer::run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
}