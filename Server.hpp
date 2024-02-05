#include <iostream>
#include <string>
#include <sys/socket.h>
Class Server {
    public:
    Server();
    Server(std::string name, std::string password, std::string port);
    Server(const Server& server);
    Server::operator=(const Server& server);
    ~Server();
    void setName(std::string name);
    
    private:
    std::string _name;
    std::string _password;
    std::string _port;
    int _socket;
};