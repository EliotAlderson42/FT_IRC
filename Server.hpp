#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <arpa/inet.h>
#include <sys/socket.h>

class Server {
    public:
    Server();
    Server(std::string name, std::string password, std::string port);
    Server(const Server& server);
    Server const &operator=(const Server& server);
    ~Server();
    void setServerSocket();
    void setServerAddr();
    void bindServer();
    void listenServer();
    int const getServerSocket();
    private:
    std::string _name;
    std::string _password;
    std::string _port;
    int _serverSocket;
    sockaddr_in _serverAddr;
};