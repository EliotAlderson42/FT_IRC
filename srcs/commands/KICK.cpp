#include "Lib.hpp"

void Server::kick(std::string str, int socket)
{
    std::vector <std::string> commands = splitCommands(str, '\n');
    std::vector <std::string>::iterator it = commands.begin();
    for (; it != commands.end(); it++)
    {
        std::string word = *it;
        std::istringstream iss(word);
        std::string channel, target;
        iss >> word >> channel >> target;
        if (_channels.find(channel) == _channels.end())
        {
            std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), channel);
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
        if (!_channels[channel]->isOperator(socket))
        {
            std::string msg = _channels[channel]->isInChannel(socket)
                              ? ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), channel.substr(1))
                              : ERR_NOTONCHANNEL(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
        if (_clients.find(getClientSocket(target)) == _clients.end())
        {
            std::string msg = ERR_USERNOTINCHANNEL(_clients[socket]->getNickname(), target, channel);
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
        int targetSocket = getClientSocket(target);
        iss >> word;
        word = word.substr(1);
        std::string msg = word.empty()  ? KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "Kicked by operator", channel, target)
                                        : KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), word, channel, target);
        _clients[targetSocket]->removeChannel(_channels[channel]);
        _channels[channel]->removeOperator(targetSocket);
        _channels[channel]->removeSocket(targetSocket);
        _channels[channel]->diffuseMessage(msg);
    }
}