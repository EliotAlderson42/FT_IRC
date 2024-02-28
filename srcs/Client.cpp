#include "Lib.hpp"

int dflt = 0;

Client::Client() : _username("User"), _nickname("User"), _password(""){
    std::cout << "Client created with default values" << std::endl;
}

Client::Client(int clientSocket, sockaddr_in clientAddr) : _clientSocket(clientSocket), _clientAddr(clientAddr) {
    this->_nickname = "nick" + toString(dflt);
    this->_username = "uname" + toString(dflt);
    dflt++;
    std::cout << "Client created with custom values" << std::endl;
}

int Client::getSocket() const {return(this->_clientSocket);}

std::string Client::getUsername() const {return(this->_username);}

void Client::setClientSocket(int socket)
{
    this->_clientSocket = socket;
}

void    Client::setIsConnect(int nb)
{
    this->_isConnect = nb;
}

int    Client::getIsConnect()
{
    return (this->_isConnect);
}

int Client::getNickLength()
{
    return (this->_nickname.size());
}

void Client::setClientAddr(sockaddr_in addr)
{
    this->_clientAddr = addr;
}

Client::Client(const Client& client) : _username(client._username), _nickname(client._nickname), _password(client._password) {
    std::cout << "Client created by copy" << std::endl;
}

Client const &Client::operator=(const Client& client) {
    this->_username = client._username;
    this->_nickname = client._nickname;
    this->_password = client._password;
    // this->_operator = client._operator;
    return *this;
}

void Client::setUsername(std::string username) {
    this->_username = username;
}

void Client::setNickname(std::string nickname) {
    this->_nickname = nickname ;
}

void Client::isExpectingNickname(bool value) {
    this->_expectingNickname = value;
}

void Client::isExpectingUsername(bool value) {
    this->_expectingUsername = value;
}

int Client::getExpectingUsername() const {return(this->_expectingUsername);}

bool Client::getExpectingNickname() const {
    return(this->_expectingNickname);
}
std::string Client::getNickname() const {return(this->_nickname);}

Client::~Client() {
    _channels.clear();
    std::cout << "Client destroyed" << std::endl;
}

void Client::addChannel(Channel *channel) {
    this->_channels.push_back(channel);
}

void Client::removeChannels() {
    std::vector<Channel *>::iterator it = this->_channels.begin();
    while (it != this->_channels.end()) {
        (*it)->removeSocket(this->_clientSocket);
        this->_channels.erase(it);
        it = this->_channels.begin();
    }
}

void Client::diffuseMessage(std::string message) {
    for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
        (*it)->sendChanMsg(this->_clientSocket, RPL_PRIVMSG_CHANNEL((RED + this->getNickname() + RESET), (*it)->getName(), message));
}

sockaddr_in Client::getClientAddr() const {return(this->_clientAddr);}

void Client::isExpectingPassword(bool value) {this->_expectingPassword = value;}
bool Client::getExpectingPassword() const {return(this->_expectingPassword);}