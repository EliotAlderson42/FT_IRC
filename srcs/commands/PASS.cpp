#include "Lib.hpp"

void Server::pass(std::string str, int socket)
{
    std::string pass = str.substr(5, this->getPassLength());
    if (pass == this->getPassword())
    {
        std::string msg = "Correct Password !\n";
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(1);
    }
    else
    {
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(0);
    }
}