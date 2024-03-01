#include "Lib.hpp"

void Server::join(std::string str, int socket)
{
    str = str.substr(5);
    str.erase(str.find_last_not_of('\n'));
    std::vector<std::string> commands = splitCommands(str, ' ');
    std::string channel = commands[0];
    std::string password;
    if (commands.size() == 2)
        password = commands[1];
    if (channel[0] == '#')
    {
		if (_channels.find(channel) != _channels.end())
        {
            if (_channels[channel]->getInviteOnly() && !_channels[channel]->isInvited(socket))
            {
                std::string msg = ERR_INVITEONLYCHAN(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_channels[channel]->getLimit() && _channels[channel]->getSockets().size() >= _channels[channel]->getLimit())
            {
                std::string msg = ERR_CHANNELISFULL(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (!_channels[channel]->getPassword().empty() && _channels[channel]->getPassword() != password)
            {
                std::string msg = ERR_BADCHANNELKEY(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_channels[channel]->isInvited(socket))
                _channels[channel]->removeInvited(socket);
           _channels[channel]->addSocket(socket);
           _clients[socket]->addChannel(_channels[channel]);
           std::string msg = RPL_JOIN(_clients[socket]->getNickname(), channel.substr(1));
           send(socket, msg.c_str(), msg.size(), 0);
           if (_channels[channel]->getTopic().empty())
           {
                std::string msg2 = RPL_NOTOPIC(_clients[socket]->getNickname(), channel.substr(1));
                send(socket, msg2.c_str(), msg2.size(), 0);
           }
           else
           {
                std::ostringstream oss;
                oss << std::time(0);
                std::string time = oss.str();
                std::string msg2 = RPL_TOPIC(_clients[socket]->getNickname(), channel.substr(1), _channels[channel]->getTopic());\
                send(socket, msg2.c_str(), msg2.size(), 0);
                std::string msg3 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), channel.substr(1), _channels[channel]->getTopicSetter(), time);
                send(socket, msg3.c_str(), msg3.size(), 0);
           }
        }
        else
        {
            _channels[channel] = new Channel(channel);
            _channels[channel]->addSocket(socket);
            _channels[channel]->setOperator(socket);
            _clients[socket]->addChannel(_channels[channel]);
			std::string msg = RPL_JOIN(_clients[socket]->getNickname(), channel.substr(1));            
    		send(socket, msg.c_str(), msg.size(), 0);
            std::string msg2 = RPL_NOTOPIC(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg2.c_str(), msg2.size(), 0);
        }
    }
}