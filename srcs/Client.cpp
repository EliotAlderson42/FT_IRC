#include "Lib.hpp"

int dflt = 0;

Client::Client() : _username("User"), _nickname("User"), _password(""){
    std::cout << "Client created with default values" << std::endl;
}

Client::Client(int clientSocket, sockaddr_in clientAddr) : _clientSocket(clientSocket), _clientAddr(clientAddr) {
    this->_nickname = "nick" + std::to_string(dflt);
    this->_username = "uname" + std::to_string(dflt);
    dflt++;
    std::cout << "Client created with custom values" << std::endl;
}

int Client::getSocket() const {return(this->_clientSocket);}

std::string Client::getUsername() const {return(this->_username);}

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

void Client::setUsername(std::string username) {
    this->_username = username;
}

void Client::setNickname(std::string nickname) {
    this->_nickname = nickname;
}

void Client::isExpectingNickname(bool value) {
    this->_expectingNickname = value;
}

void Client::isExpectingUsername(bool value) {
    this->_expectingUsername = value;
}

bool Client::getExpectingUsername() const {return(this->_expectingUsername);}

bool Client::getExpectingNickname() const {
    return(this->_expectingNickname);
}
std::string Client::getNickname() const {return(this->_nickname);}

Client::~Client() {
    std::cout << "Client destroyed" << std::endl;
}

sockaddr_in Client::getClientAddr() const {return(this->_clientAddr);}

void Client::isExpectingPassword(bool value) {this->_expectingPassword = value;}
bool Client::getExpectingPassword() const {return(this->_expectingPassword);}