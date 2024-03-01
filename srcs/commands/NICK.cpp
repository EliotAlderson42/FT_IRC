#include "Lib.hpp"

void Server::nick(std::string str, int socket)
{
    std::string sub = str.substr(5, 6);
    if (sub.empty())
    {
        std::string msg = ERR_NONICKNAMEGIVEN(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else if (sub.find("#") != std::string::npos || sub.find(":") != std::string::npos || sub.find(" ") != std::string::npos || sub.find("\t") != std::string::npos || sub.find("\r") != std::string::npos || sub.find("\n") != std::string::npos || sub.find("\v") != std::string::npos || sub.find("\f") != std::string::npos)
    {
        std::string msg = ERR_ERRONEUSNICKNAME(sub);
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            if (it->second->getNickname() == sub)
            {
                std::string msg = ERR_NICKNAMEINUSE(sub);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
        }
        _clients[socket]->setNickname(sub);
        std::string msg = ":localhost 001 " + sub + " :Nickname changed to " + sub + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
    }
}