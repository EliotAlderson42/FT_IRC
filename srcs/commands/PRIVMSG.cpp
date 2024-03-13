#include "Lib.hpp"

void Server::privmsg(std::string str, int socket)
{
	std::istringstream iss(str);
	std::string word;
	iss >> word;
	iss >> word;

	std::string toSend;
    size_t pos = str.find(':');
    if (pos != std::string::npos)
        toSend = str.substr(pos);
    else
    {
        std::string msg = ERR_NOTEXTTOSEND(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (word[0] == '#')
	{
        if ((_channels.find(word) == _channels.end()) || (!_channels[word]->isInChannel(socket)))
	    {
            std::string msg = ERR_CANNOTSENDTOCHAN(_clients[socket]->getNickname(), word);
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
    	std::string msg = RPL_PRIVMSG_CHANNEL((CYAN + _clients[socket]->getNickname() + RESET), word, toSend);
		_channels[word]->sendChanMsg(socket, msg);
        return ;
	}
    else
    {
        for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            if (it->second->getNickname() == word)
            {
    	        std::string msg = RPL_PRIVMSG_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), word, toSend);
                // std::cout << msg << std::endl;
                send(it->second->getSocket(), msg.c_str(), msg.size(), 0);
                return ;
            }
        }
	}
    std::string msg = _clients[socket]->getNickname() + " :No user with this name\r\n";
    send(socket, msg.c_str(), msg.size(), 0);
}