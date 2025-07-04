#include "../headers/bridge.h"

WebSocketServer::WebSocketServer() {
    m_server.init_asio();
    user_manager = new WSUserManager();

    m_server.set_open_handler([](connection_hdl hdl){
        std::cout << "Client connected\n";
    });

    m_server.set_close_handler([](connection_hdl hdl){
        std::cout << "Client disconnected\n";
    });

    m_server.set_message_handler([this](connection_hdl hdl, const server::message_ptr& msg){
        auto payload = msg->get_payload();
        auto shared = hdl.lock();
        std::string content;
        if(!shared)
            return;

        if(payload.find(SAFEGUARDS::LOGIN) != std::string::npos){
            std::string login = payload.substr(SAFEGUARDS::LOGIN.size(), payload.size() - SAFEGUARDS::LOGIN.size());
            user_manager->add(shared, login);
        }

        if(payload.find(SAFEGUARDS::CONTENT) != std::string::npos){
            content = payload.substr(SAFEGUARDS::CONTENT.size(), payload.size() - SAFEGUARDS::CONTENT.size());
        }
        auto user = user_manager->getUser(shared);
        if(user){
            std::cout << "Received Message from->: " <<  user->login << std::endl;
            std::cout << payload << std::endl;
        }

        if(content.size() && user)
            client->sendFrom(content, user->login);
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
        connectToServer(port + 1);
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
}

void WebSocketServer::connectToServer(int server_port) {
    std::string ip_addr = "192.168.31.49";
    client = new http::TCPClient(ip_addr, server_port);
    sockaddr_in si;
    si.sin_family = AF_INET;
    si.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    si.sin_port = htons(server_port);
    client->setServerAddress((sockaddr *) &si, sizeof(si));
    client->connect();
    client->start_listen();
    message_handler_thread = new std::thread(&WebSocketServer::handle_messages, this);
    message_handler_thread->detach();
}

void WebSocketServer::handle_messages() {
    while(true){
        auto msg = client->retriveLastMessage();
        if(msg){
            auto from = msg->getFrom();
            std::string content = from + ": " + *msg->getContent();
            auto to = msg->getTo();
            if(to == "all"){
                auto users = user_manager->getUsers();
                for(auto& [hdl, user]: *users){
                    m_server.send(hdl, content, websocketpp::frame::opcode::text);
                }
            } else{
                m_server.send(user_manager->getHandle(to), content, websocketpp::frame::opcode::text);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}