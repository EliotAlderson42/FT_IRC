#include "Server.hpp"

Server::Server() : _name("localhost"), _password(""), _port("6667"), _socket(0) {
    socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }
    std::cout << "Server created with default values" << std::endl;
}

Server::Server(std::string name, std::string password, std::string port) : _name("localhost"), _password(password), _port(port), _socket(0) {
    socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }

    std::cout << "Server created with custom values" << std::endl;
}

Server::Server(const Server& server) : _name(server._name), _password(server._password), _port(server._port), _socket(server._socket) {
    std::cout << "Server created with copy constructor" << std::endl;
}

Server::operator=(const Server& server) {
    _name = server._name;
    _password = server._password;
    _port = server._port;
    _socket = server._socket;
    std::cout << "Server created with copy assignment" << std::endl;
}

Server::~Server() {
    close(_socket);
    std::cout << "Server destroyed" << std::endl;
}

void Server::setName(std::string name) {this->_name = name;}
