#pragma once
#include "Lib.hpp"

class Server;
class Client {
    public:
    Client();
    Client(int _clientSocket, sockaddr_in _clientAddr, Server *clientServ);
    Client(const Client& client);
    Client const &operator=(const Client& client);
    ~Client();

    sockaddr_in  getClientAddr() const;
    void setClientSocket(int socket);
    void setClientAddr(sockaddr_in addr);
    int getSocket() const;
    std::string getNickname() const;
    void setNickname(std::string nickname);
    void isExpectingNickname(bool value);
    bool getExpectingNickname() const;
    void isExpectingPassword(bool value);
    bool getExpectingPassword() const;

    private:
    std::string _username;
    std::string _nickname;
    std::string _password;
    ssize_t _dataSent;
    int destination;
    int channel;
    ssize_t _dataReceived;
    bool _operator = false;
    int _clientSocket;
    sockaddr_in _clientAddr;
    Server *_clientServer;
    bool _expectingNickname = false;
    bool _expectingPassword = false;
};