#include "Lib.hpp"

void Server::list(std::string str, int socket)
{
    std::istringstream iss(str.substr(5));
    std::string word;
    iss >> word;

    std::string start = RPL_LISTSTART(_clients[socket]->getNickname());
    std::string end = RPL_LISTEND(_clients[socket]->getNickname());
    send(socket, start.c_str(), start.size(), 0);
    if (!word.empty())
    {
        std::vector<std::string> channels = splitCommands(word, ',');
        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            *it = '#' + *it;
            if (_channels.find(*it) != _channels.end())
            {
                std::ostringstream oss;
                oss << _channels[*it]->getSockets().size();
                std::string socketsSizeStr = oss.str();
                std::string msg = RPL_LIST(_clients[socket]->getNickname(), (*it).substr(1), socketsSizeStr, _channels[*it]->getTopic());
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
    }
    else
    {
        for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            std::ostringstream oss;
            oss << it->second->getSockets().size();
            std::string socketsSizeStr = oss.str();
            std::string msg = RPL_LIST(_clients[socket]->getNickname(), it->first.substr(1), socketsSizeStr, it->second->getTopic());
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    send(socket, end.c_str(), end.size(), 0);
}