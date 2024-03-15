#include "Lib.hpp"

void Server::oMode(bool mode, std::string channel, std::string nickname, int socket)
{
    if (nickname.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (_clients.find(getClientSocket(nickname)) == _clients.end())
    {
        std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname(), nickname);
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    else if (!_channels[channel]->isInChannel(getClientSocket(nickname)))
    {
        std::string msg = ERR_NOTONCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_NEWOPER(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "o", nickname)
                           : UNSET_OPER(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "o", nickname);
    mode ? _channels[channel]->setOperator(getClientSocket(nickname)) 
         : _channels[channel]->removeOperator(getClientSocket(nickname));
    _channels[channel]->diffuseMessage(msg);
}

void Server::iMode(bool mode, std::string channel, std::string nickname, int socket)
{
    (void)nickname;
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "i")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "i");
    _channels[channel]->setInviteOnly(mode);
    _channels[channel]->diffuseMessage(msg);
}

void Server::tMode(bool mode, std::string channel, std::string nickname, int socket)
{
    (void)nickname;
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "t")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "t");
    _channels[channel]->setRestrictedTopic(mode);
    _channels[channel]->diffuseMessage(msg);
}

void Server::kMode(bool mode, std::string channel, std::string password, int socket)
{
    if (password.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "k")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "k");
    mode ? _channels[channel]->setPassword(password)
         : _channels[channel]->setPassword("");
    _channels[channel]->diffuseMessage(msg);
}

void Server::lMode(bool mode, std::string channel, std::string limit, int socket)
{
    int lim = atoi(limit.c_str());
    if (mode && !lim)
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "l")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "l");
                      mode ? _channels[channel]->setLimit(lim)
                           : _channels[channel]->setLimit(0);
    _channels[channel]->diffuseMessage(msg);
}

void Server::mode(std::string str, int socket)
{
    str = str.substr(5);
    // size_t lastChar = str.find_last_not_of(" \n\r\t");
    // if(lastChar != std::string::npos)
        // str = str.substr(0, lastChar + 1);
    std::vector<std::string> commands = splitCommands(str, ' ');
    if(!(commands.size() <= 3 && commands.size() >= 2))
       return ;
    std::string channel = commands[0];
    std::string mode = commands[1];
    std::cout << "mode = " << mode << std::endl;
    std::string nickname;
    if (commands.size() == 3)
        nickname = commands[2];
    if (_channels.find(channel) != _channels.end())
    {
        if (!_channels[channel]->isOperator(socket))
        {
            std::string msg = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
        if ((_modes.find(std::string(1, mode[1])) != _modes.end()) && mode.size() == 2)
        {
            mode[0] == '+' ? (this->*_modes[std::string(1, mode[1])])(1, channel, nickname, socket)
                           : (this->*_modes[std::string(1, mode[1])])(0, channel, nickname, socket);
        }
        else
        {
            std::string msg = ERR_UNKNOWNMODE(_clients[socket]->getNickname(), mode);
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}