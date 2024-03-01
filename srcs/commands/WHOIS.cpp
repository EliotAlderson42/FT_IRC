#include "Lib.hpp"

void    Server::whois(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    if (word == _clients[socket]->getNickname())
    {
        std::string msg = RPL_WHOISUSER(_clients[socket]->getNickname(), this->getName(), _clients[socket]->getUsername());
        send(socket, msg.c_str(), msg.size(), 0);
        msg = RPL_ENDOFWHOIS(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname() , word);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}