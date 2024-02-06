#pragma once
#include <string>
#include <iostream>

class Client {
    public:
    Client();
    Client(std::string _username, std::string _nickname, std::string _password);
    Client(const Client& client);
    Client const &operator=(const Client& client);
    ~Client();

    private:
    std::string _username;
    std::string _nickname;
    std::string _password;
    bool _operator;
    int _clientSocket;
    sockaddr_in _clientAddr;
};