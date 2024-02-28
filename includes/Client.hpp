#pragma once
#include "Lib.hpp"

class Server;
class Channel;
class Client {

    public:
    
    //Constructors and destructors
    Client(int _clientSocket, sockaddr_in _clientAddr);
    Client const &operator=(const Client& client);
    Client(const Client& client);
    ~Client();
    Client();

    //Getters
    sockaddr_in getClientAddr() const;
    std::string getUsername()   const;
    std::string getNickname()   const; 
    int         getIsConnect()  const;
    int         getSocket()     const;

    //Setters
    void setNickname(std::string nickname);
    void setUsername(std::string username);
    void setIsConnect(int nb);
    
    //Methods  
    void diffuseMessage(std::string message);   
    void addChannel(Channel *channel); 
    void removeAllChannels();
    void removeChannel(Channel *channel);
    
    private:
    sockaddr_in             _clientAddr;
    std::vector <Channel *> _channels;
    std::string             _username;
    std::string             _nickname;
    std::string             _password;
    int                     _isConnect;
    int                     _clientSocket;
};