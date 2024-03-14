#include "Lib.hpp"

int dflt = 0;



//Constructors and destructors

Client::~Client()
{
    _channels.clear();
}

Client const &Client::operator=(const Client& client)
{
    this->_username = client._username;
    this->_nickname = client._nickname;
    this->_password = client._password;
    return *this;
}

Client::Client(int clientSocket, sockaddr_in clientAddr, int isPasswd) : _clientAddr(clientAddr), _clientSocket(clientSocket)
{
    _nickname = "nick" + toString(dflt);
    _username = "uname" + toString(dflt);
    dflt++;
    _isConnect = 1;
    if (isPasswd == 1)
        _isConnect = 0;
}

Client::Client() : _username("User"), _nickname("User"), _password(""){}

Client::Client(const Client& client) : _username(client._username), _nickname(client._nickname), _password(client._password){}



//Getters

int Client::getSocket()             const
{
    return(this->_clientSocket);
}

int    Client::getIsConnect()       const
{
    return (this->_isConnect);
}

std::string Client::getNickname()   const
{
    return(this->_nickname);
}

std::string Client::getUsername()   const
{
    return(this->_username);
}

sockaddr_in Client::getClientAddr() const 
{
    return(this->_clientAddr);
}



//Setters

void    Client::setIsConnect(int nb)
{
    this->_isConnect = nb;
}

void Client::setUsername(std::string username)
{
    this->_username = username;
}

void Client::setNickname(std::string nickname)
{
    this->_nickname = nickname ;
}



//Methods

void Client::addChannel(Channel *channel) 
{
    this->_channels.push_back(channel);
}

void Client::removeChannel(Channel *channel) 
{
    std::vector<Channel *>::iterator it = this->_channels.begin();
    while (it != this->_channels.end()) 
    {
        if ((*it) == channel) 
        {
            this->_channels.erase(it);
            return;
        }
        it++;
    }
}

void Client::removeAllChannels() 
{
    std::vector<Channel *>::iterator it = this->_channels.begin();
    while (it != this->_channels.end()) 
    {
        (*it)->removeSocket(this->_clientSocket);
        this->_channels.erase(it);
        it = this->_channels.begin();
    }
}

void Client::diffuseMessage(std::string message) 
{
    for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
        (*it)->sendChanMsg(this->_clientSocket, RPL_PRIVMSG_CHANNEL((RED + this->getNickname() + RESET), (*it)->getName(), message));
}