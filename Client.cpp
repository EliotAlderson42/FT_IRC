#include "Lib.hpp"


Client::Client() : _username("default"), _nickname("default"), _password(""){
    std::cout << "Client created with default values" << std::endl;
}

Client::Client(std::string username, std::string nickname, std::string password, Server *clientServ) : _username(username), _nickname(nickname), _password(password), _clientServer(clientServ) {
    std::cout << "Client created with custom values" << std::endl;
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
