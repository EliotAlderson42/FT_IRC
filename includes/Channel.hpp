#pragma once
#include "Lib.hpp"

class Channel {
    public:
    Channel();
    Channel(std::string name);
    ~Channel();
    // void kickClient(int clientSocket);
    // void inviteClient(Client *clientSocket);
    // void Mode();
    // void getClient (int clientSocket);
    // void diffuseMessage(std::string message);
    // Client *findClient(int clientSocket);
    void    addSocket(int socket);
    std::string getPassword() const;
    private:
    std::string _name;
    std::string _password;
    std::vector<int> _sockets;
    // std::unordered_map<int, Client *> _clients;
};