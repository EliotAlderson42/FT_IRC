#include "Lib.hpp"


Client::Client() : _username("default"), _nickname("default"), _password(""){
    std::cout << "Client created with default values" << std::endl;
}

Client::Client(int clientSocket, sockaddr_in clientAddr, Server *clientServ) : _clientSocket(clientSocket), _clientAddr(clientAddr), _clientServer(clientServ) {
    std::cout << "Client created with custom values" << std::endl;
}

int Client::getSocket() const {return(this->_clientSocket);}

void Client::setClientSocket(int socket)
{
    this->_clientSocket = socket;
}

void Client::setClientAddr(sockaddr_in addr)
{
    this->_clientAddr = addr;
}

Client::Client(const Client& client) : _username(client._username), _nickname(client._nickname), _password(client._password), _operator(client._operator) {
    std::cout << "Client created by copy" << std::endl;
}

Client const &Client::operator=(const Client& client) {
    this->_username = client._username;
    this->_nickname = client._nickname;
    this->_password = client._password;
    this->_operator = client._operator;
    return *this;
}

void Client::setNickname(std::string nickname) {
    this->_nickname = nickname;
}

void Client::isExpectingNickname(bool value) {
    this->_expectingNickname = value;
}
bool Client::getExpectingNickname() const {
    return(this->_expectingNickname);
}
std::string Client::getNickname() const {return(this->_nickname);}

Client::~Client() {
    std::cout << "Client destroyed" << std::endl;
}

sockaddr_in Client::getClientAddr() const {return(this->_clientAddr);}
