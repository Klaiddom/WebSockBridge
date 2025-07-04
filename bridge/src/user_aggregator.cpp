#include "../headers/user_aggregator.h"

WSUser::WSUser(websocketpp::connection_hdl hdl, std::string& login) {
    handle = hdl;
    this->login = login;
}


void WSUserManager::add(std::shared_ptr<void> hdl, std::string& login) {
    std::lock_guard<std::mutex> user_lock(users_mutex);
    std::lock_guard<std::mutex> user2handle_lock(user2handle_mutex);
    if(users.find(hdl) == users.end()){
        users[hdl] = new WSUser(hdl, login);
        user2handle[login] = hdl;
    }
}

WSUser* WSUserManager::getUser(connection_hdl username){
    std::lock_guard<std::mutex> lock(users_mutex);
    if(users.find(username) == users.end())
        return nullptr;
    return users[username];
}

void WSUserManager::removeUser(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(users_mutex);
    if(users.find(hdl) != users.end())
        users.erase(hdl);
}

connection_hdl WSUserManager::getHandle(std::string &username) {
    std::lock_guard<std::mutex> user2handle_lock(user2handle_mutex);
    if(user2handle.find(username) != user2handle.end())
        return user2handle[username];
    return std::weak_ptr<void>();
}

std::map<connection_hdl , WSUser*, std::owner_less<connection_hdl> >* WSUserManager::getUsers(){
    return &users;
}
