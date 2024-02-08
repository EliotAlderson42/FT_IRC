#pragma once
#include "Lib.hpp"

class Client;
class Server {
    public:
    Server();
    Server(std::string name, std::string password, std::string port);
    ~Server();
    
    //Setters and methods
    void setServerSocket();
    void setServerAddr();
    void bindServer();
    void listenServer();
    int nbClients;
    
    //Getters
    std::string getName();
    std::string getPassword();
    int const getServerSocket();
    
    private:
    std::string _name;
    std::string _password;
    std::string _port;
    int _serverSocket = 0;
    sockaddr_in _serverAddr;
};