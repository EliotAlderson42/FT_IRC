#include "Lib.hpp"

void Server::invite(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    std::string target = word;
    iss >> word;
    std::string channel = word;
    
    if (_channels.find(channel) != _channels.end())
    {
        if (_channels[channel]->isOperator(socket))
        {
            if (getClientSocket(target) && !_channels[channel]->isInChannel(getClientSocket(target)))
            {
                _channels[channel]->setInvited(getClientSocket(target));
                std::string msg = RPL_INVITING(_clients[socket]->getNickname(), target, channel);
                std::string msg2 = INVITE_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "INVITE", target, channel);
                send(socket, msg.c_str(), msg.size(), 0);
                send(getClientSocket(target), msg2.c_str(), msg2.size(), 0);
            }
            else if(_channels[channel]->isInChannel(getClientSocket(target)))
            {
                std::string msg = ERR_USERONCHANNEL(target, channel);
                send(socket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname(), target);
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            std::string msg = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}