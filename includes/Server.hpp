#pragma once
#include "Lib.hpp"

class Client;
class Channel;
class Server {
    public:
    Server();
    Server(std::string name, std::string password, std::string port);
    ~Server();
    typedef void (*FunctionPointer)();
    
    //Setters and methods
    void setServerSocket();
    void setServerAddr();
    void bindServer();
    void listenServer();
    int nbClients;
    
    //command fonctions
    void    join(std::string str, int socket);
    void    privmsg(std::string str, int socket);
    void    pong(std::string str, int socket);
    void    whois(std::string str, int socket);
    void    capls(std::string str, int socket);
    void    pass(std::string str, int socket);
    void    nick(std::string str, int socket);
    void    user(std::string str, int socket);
    //Getters
    std::string getName();
    std::string getPassword();
    int const getServerSocket();

    //utils
    int    initServ();
    void    mainLoop();
    int    addClient();
    void    removeClient(int socket);

    private:
    std::string _name;
    std::string _password;
    std::string _port;
    int _nbChannels;
    int _serverSocket;
    int _epollFd;
    int _numEvents;
    sockaddr_in _serverAddr;
    std::map<std::string, void(Server::*)(std::string, int)> _funcTab;
    std::map<int, Client *> _clients;
    std::map<std::string, Channel *> _channels;
    std::vector<struct epoll_event> _events;// je cree la structure de gestion des events
};