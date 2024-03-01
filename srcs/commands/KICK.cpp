#include "Lib.hpp"

void Server::kick(std::string str, int socket)
{
    std::vector <std::string> commands = splitCommands(str, '\n');
    std::vector <std::string>::iterator it = commands.begin();
    for (; it != commands.end(); it++)
    {
        std::string word = *it;
        std::istringstream iss(word);
        iss >> word;
        iss >> word;
        if (_channels.find(word) != _channels.end())
        {
            if(_channels[word]->isOperator(socket))
            {
                std::string channel = word;
                iss >> word;
                std::string target = word;
                int targetSocket = getClientSocket(target);
                if (_channels[channel]->isInChannel(targetSocket))
                {
                    iss >> word;
                    word = word.substr(1);   
                    std::string msg = word.empty()  ? KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "Kicked by operator", channel, target)
                                                    : KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), word, channel, target);
                    _clients[targetSocket]->removeChannel(_channels[channel]);
                    _channels[channel]->removeOperator(targetSocket);
                    _channels[channel]->removeSocket(targetSocket);     
                    _channels[channel]->diffuseMessage(msg);

                }
                else
                {
                    std::string msg = ERR_NOTONCHANNEL(_clients[targetSocket]->getNickname(), word);
                    send(socket, msg.c_str(), msg.size(), 0);
                }
            }
            else
            {
                std::string msg = _channels[word]->isInChannel(socket) 
                                  ? ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), word.substr(1))
                                  : ERR_NOTONCHANNEL(_clients[socket]->getNickname(), word);
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), word);
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
}