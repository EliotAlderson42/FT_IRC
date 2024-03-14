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
    void setIsPasswd(int nb);
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
    void    quit(std::string str, int socket);
    void    topic(std::string str, int socket);
    void    kick(std::string str, int socket);
    void    invite(std::string str, int socket);
    void    list(std::string str, int socket);
    void    mode(std::string str, int socket);


    
    //Mode functions

    void    oMode(bool mode, std::string channel, std::string nickname, int socket);
    void    iMode(bool mode, std::string channel, std::string nickname, int socket);
    void    tMode(bool mode, std::string channel, std::string nickname, int socket);
    void    kMode(bool mode, std::string channel, std::string password, int socket);
    void    lMode(bool mode, std::string channel, std::string limit, int socket);
    //Getters  
    std::string getName();
    std::string getPassword();
    int getServerSocket();
    int getPassLength();
    int getNickLength();
    int getClientSocket(std::string nickname);
    int getIsPasswd();
    //utils
    int    initServ();
    void    mainLoop();
    int    addClient();
    void    removeClient(int socket);
    std::map<int, Client *> getClients();
    std::map<std::string, Channel *> getChannels();
    Channel *getChannel(std::string name);

    private:
    std::string _name;
    std::string _password;
    std::string _port;
    int _nbChannels;
    int _serverSocket;
    int _epollFd;
    int _numEvents;
    int _isPasswd;
    sockaddr_in _serverAddr;
    std::map<std::string, void(Server::*)(std::string, int)> _funcTab;
    std::map<int, Client *> _clients;
    std::map<std::string, Channel *> _channels;
    std::vector<struct epoll_event> _events;// je cree la structure de gestion des events
    std::map<std::string, void(Server::*)(bool, std::string, std::string, int)> _modes;
};