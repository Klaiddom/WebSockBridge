#ifndef WINSOCKBRIDGE_USER_AGGREGATOR_H
#define WINSOCKBRIDGE_USER_AGGREGATOR_H

#include <map>
#include <mutex>
#include <websocketpp/config/asio_no_tls.hpp>

using websocketpp::connection_hdl;

struct WSUser{
    connection_hdl handle;
    std::string login = "";

    WSUser(connection_hdl hdl, std::string& login);
};

class WSUserManager{
    std::map<connection_hdl , WSUser*, std::owner_less<connection_hdl> > users;
    std::map<std::string, connection_hdl > user2handle;
    std::mutex users_mutex;
    std::mutex user2handle_mutex;

public:
    WSUserManager() = default;
    void add(std::shared_ptr<void> hdl, std::string& login);
    WSUser* getUser(connection_hdl username);
    connection_hdl getHandle(std::string& username);
    void removeUser(connection_hdl hdl);
    std::map<connection_hdl , WSUser*, std::owner_less<connection_hdl> >* getUsers();
};


#endif