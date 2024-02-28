#include "Lib.hpp"

Channel::Channel() : _name("default"), _password("") {
    std::cout << "Channel created with default values" << std::endl;
}

Channel::Channel(std::string name) : _name(name){
    std::cout << "Channel created with custom values" << std::endl;
}

Channel::~Channel() {
    std::vector<int>::iterator it = this->_sockets.begin();
    while (it != this->_sockets.end())
    {
        close(*it);
        it = this->_sockets.erase(it);
    }
    std::cout << "Channel destroyed" << std::endl;
}

void    Channel::addSocket(int socket)
{
    this->_sockets.push_back(socket);
}

void    Channel::setOperator(int socket)
{
    if (std::find(_operators.begin(), _operators.end(), socket) == _operators.end()) {
        _operators.push_back(socket);
    }
}

std::string Channel::getTopic() const {return this->_topic;}

void   Channel::setTopic(std::string topic){this->_topic = topic;}

void   Channel::setTopicSetter(Client *client){
    struct sockaddr_in clientAddr = client->getClientAddr();
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN);
    this->_topicSetter = client->getNickname() + "!" + client->getUsername() + "@" + ip;}

std::string Channel::getTopicSetter() const {return this->_topicSetter;}

std::string Channel::getName() const {return this->_name;}

std::vector<int> Channel::getSockets() const {
    std::vector<int> sockets;
    for (std::vector<int>::const_iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
        sockets.push_back(*it);
    return sockets;
}

bool   Channel::isInChannel(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_sockets.begin(), this->_sockets.end(), socket);
    if (it != this->_sockets.end())
        return true;
    return false;
}

void   Channel::removeSocket(int socket)
{
    std::vector<int>::iterator it = std::find(this->_sockets.begin(), this->_sockets.end(), socket);
    if (it != this->_sockets.end())
        this->_sockets.erase(it);
}

void  Channel::removeOperator(int socket)
{
    std::vector<int>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), socket);
    if (it != this->_operators.end())
        this->_operators.erase(it);
}

bool    Channel::isOperator(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_operators.begin(), this->_operators.end(), socket);
    if (it != this->_operators.end())
        return true;
    return false;
}

bool   Channel::getRestrictedTopic() const {return this->_restrictedTopic;}

void    Channel::setRestrictedTopic(bool value){this->_restrictedTopic = value;}

void    Channel::setInvited(int socket){this->_invited.push_back(socket);}

void    Channel::setInviteOnly(bool value){this->_inviteOnly = value;}

bool    Channel::getInviteOnly() const {return this->_inviteOnly;}

void    Channel::removeInvited(int socket)
{
    std::vector<int>::iterator it = std::find(this->_invited.begin(), this->_invited.end(), socket);
    if (it != this->_invited.end())
        this->_invited.erase(it);
}

bool    Channel::isInvited(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_invited.begin(), this->_invited.end(), socket);
    if (it != this->_invited.end())
        return true;
    return false;
}

void    Channel::sendChanMsg(int sock, std::string msg)
{
    for (std::vector<int>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
    {
        if (*it != sock)
            send(*it, msg.c_str(), msg.size(), 0);
    }
}

void    Channel::diffuseMessage(std::string msg)
{
    for (std::vector<int>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
        send(*it, msg.c_str(), msg.size(), 0);
}

std::string Channel::getPassword() const {return this->_password;}

void    Channel::setPassword(std::string password){this->_password = password;}

void    Channel::setLimit(int limit){this->_limit = limit;}

int    Channel::getLimit() const {return this->_limit;}

// void    Channel::sendGrpMsg(std::string str)
// {
//     std::string toSend;
//     size_t pos = str.find(':');
//     if (pos != std::string::npos)
//         toSend = str.substr(pos);
//     // else
//     // {
//     //     \\\\\\
//     //     \\\\\\
//     //     \\\\\\
//     // }
//     std::string msg = RPL_PRIVMSG_CHANNEL()
// }

// void Channel::inviteClient(Client *client) {
//     _clients[client->getSocket()] = client;
//     std::cout << "Client invited to channel" << std::endl;
// }

// void Channel::diffuseMessage(std::string message) {
//     for (auto it = _clients.begin(); it != _clients.end(); it++) {
//         message.append("\n");
//         send(it->first, message.c_str(), message.size(), 0);
//     }
// }


// Client *Channel::findClient(int clientSocket) {
//     return _clients[clientSocket];
// }