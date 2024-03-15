#include "Lib.hpp"

void Server::topic(std::string str, int socket)
{
    std::istringstream iss(str);
    std::ostringstream oss;
    oss << std::time(0);
    std::string word;
    iss >> word;
    iss >> word;
    std::string currentTime = oss.str();

    if (_channels.find(word) != _channels.end())
    {
        std::string toSend;
        size_t pos = str.find(':');
        if (pos != std::string::npos)
        {
            if (_channels[word]->getRestrictedTopic() /*&& !_channels[word]->isOperator(socket)*/)
            {
                toSend = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), word.substr(1));
                send(socket, toSend.c_str(), toSend.size(), 0);
                return ;
            }
            _channels[word]->setTopic(str.substr(pos + 1));
            _channels[word]->setTopicSetter(_clients[socket]);
            std::string msg = RPL_TOPIC(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopic());
            std::string msg2 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopicSetter(), currentTime);
            std::vector<int> sockets = _channels[word]->getSockets();
            for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
            {
                send(*it, msg.c_str(), msg.size(), 0);
                send(*it, msg2.c_str(), msg2.size(), 0);
            }
        }
        else
        {

            if (_channels[word]->getTopic().empty())
            {
                std::string msg = RPL_NOTOPIC(_clients[socket]->getNickname(), word.substr(1));
                send(socket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                std::string msg = RPL_TOPIC(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopic());
                std::string msg2 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopicSetter(), currentTime);
                send(socket, msg.c_str(), msg.size(), 0);
                send(socket, msg2.c_str(), msg2.size(), 0);
            }
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), word);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}