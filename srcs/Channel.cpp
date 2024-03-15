#include "Lib.hpp"

//Constructors and destructors

Channel::Channel() : _password(""), _name("default") {}

Channel::Channel(std::string name) : _name(name)
{
    _limit = 0;
    this->_restrictedTopic = 0;
}

Channel::~Channel()
{
    std::vector<int>::iterator it = this->_sockets.begin();
    while (it != this->_sockets.end())
    {
        close(*it);
        it = this->_sockets.erase(it);
    }
}



//Getters

size_t    Channel::getLimit()           const
{
    return this->_limit;
}

std::string Channel::getName()          const 
{
    return this->_name;
}

std::string Channel::getTopic()         const 
{
    return this->_topic;
}

bool    Channel::getInviteOnly()        const
{
    return this->_inviteOnly;
}

std::string Channel::getPassword()      const 
{
    return this->_password;
}

bool   Channel::getRestrictedTopic()    const 
{
    return this->_restrictedTopic;
}

std::string Channel::getTopicSetter()   const 
{
    return this->_topicSetter;
}

std::vector<int> Channel::getSockets()  const 
{
    std::vector<int> sockets;
    for (std::vector<int>::const_iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
        sockets.push_back(*it);
    return sockets;
}



//Setters

void    Channel::setLimit(int limit)
{
    this->_limit = limit;
}

void    Channel::setInvited(int socket)
{
    this->_invited.push_back(socket);
}

void    Channel::setInviteOnly(bool value)
{
    this->_inviteOnly = value;
}

void   Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

void    Channel::setRestrictedTopic(bool value)
{
    this->_restrictedTopic = value;
}

void    Channel::setPassword(std::string password)
{
    this->_password = password;
}

void    Channel::setOperator(int socket)
{
    if (std::find(_operators.begin(), _operators.end(), socket) == _operators.end()) {
        _operators.push_back(socket);
    }
}

void   Channel::setTopicSetter(Client *client)
{
    struct sockaddr_in clientAddr = client->getClientAddr();
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN);
    this->_topicSetter = client->getNickname() + "!" + client->getUsername() + "@" + ip;
}



//Methods

void    Channel::addSocket(int socket)
{
    this->_sockets.push_back(socket);
}

void    Channel::diffuseMessage(std::string msg)
{
    for (std::vector<int>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
        send(*it, msg.c_str(), msg.size(), 0);
}

void    Channel::sendChanMsg(int sock, std::string msg)
{
    for (std::vector<int>::iterator it = this->_sockets.begin(); it != this->_sockets.end(); it++)
    {
        if (*it != sock)
            send(*it, msg.c_str(), msg.size(), 0);
    }
}

void    Channel::removeInvited(int socket)
{
    std::vector<int>::iterator it = std::find(this->_invited.begin(), this->_invited.end(), socket);
    if (it != this->_invited.end())
        this->_invited.erase(it);
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

bool   Channel::isInChannel(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_sockets.begin(), this->_sockets.end(), socket);
    if (it != this->_sockets.end())
        return true;
    return false;
}

bool   Channel::isInvited(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_invited.begin(), this->_invited.end(), socket);
    if (it != this->_invited.end())
        return true;
    return false;
}

bool   Channel::isOperator(int socket) const
{
    std::vector<int>::const_iterator it = std::find(this->_operators.begin(), this->_operators.end(), socket);
    if (it != this->_operators.end())
        return true;
    return false;
}