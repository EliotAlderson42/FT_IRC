#pragma once
#include "Lib.hpp"

class Server;
class Channel;
class Client {
    public:
    Client();
    Client(int _clientSocket, sockaddr_in _clientAddr);
    Client(const Client& client);
    Client const &operator=(const Client& client);
    ~Client();

    sockaddr_in  getClientAddr() const;
    void setClientAddr(sockaddr_in addr);
    void setClientSocket(int socket);
    int getSocket() const;
    std::string getNickname() const;
    void setNickname(std::string nickname);
    void isExpectingNickname(bool value);
    bool getExpectingNickname() const;
    std::string getUsername() const;
    void isExpectingPassword(bool value);
    bool getExpectingPassword() const;
    void setUsername(std::string username);
    void isExpectingUsername(bool value);
    int getExpectingUsername() const;
    void setIsConnect(int nb);
    int getIsConnect();
    int getNickLength();
    void addChannel(Channel *channel);
    void removeChannels();
    void diffuseMessage(std::string message);


    private:
    std::string _username;
    std::string _nickname;
    std::string _password;
    ssize_t _dataSent;
    int _isConnect = 0;
    int destination;
    int channel;
    ssize_t _dataReceived;
    int _clientSocket;
    sockaddr_in _clientAddr;
    // Server *_clientServer;
    bool _expectingNickname = false;
    int _expectingUsername = 1;
    bool _expectingPassword = false;
    std::vector <Channel *> _channels;
};