#pragma once
#include "Lib.hpp"

class Server;
class Client {
    public:
    Client();
    Client(std::string username, std::string nickname, std::string password, Server *clientServ);
    Client(const Client& client);
    Client const &operator=(const Client& client);
    ~Client();


    sockaddr_in  getClientAddr() const;

    private:
    std::string _username;
    std::string _nickname;
    std::string _password;
    ssize_t _dataSent;
    ssize_t _dataReceived;
    bool _operator = false;
    sockaddr_in _clientAddr;
    Server *_clientServer;
};