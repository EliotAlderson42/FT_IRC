#include "Lib.hpp"

void Server::user(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    if (word.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "USER");
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        _clients[socket]->setUsername(word);
        std::string msg = "Username changed to " + word + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
    }
}