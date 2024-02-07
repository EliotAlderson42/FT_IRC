#include "Lib.hpp"


Client::Client() : _username("default"), _nickname("default"), _password(""){
    std::cout << "Client created with default values" << std::endl;
}

Client::Client(int clientSocket, sockaddr_in clientAddr, Server *clientServ) : _clientSocket(clientSocket), _clientAddr(clientAddr), _clientServer(clientServ) {
    std::cout << "Client created with custom values" << std::endl;
}

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

Client::~Client() {
    std::cout << "Client destroyed" << std::endl;
}

sockaddr_in Client::getClientAddr() const {return(this->_clientAddr);}
