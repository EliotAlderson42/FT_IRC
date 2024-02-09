#include "Lib.hpp"

Server::Server() : _name("localhost"), _password(""), _port("6667"), _serverSocket(0) {
    setServerSocket();
    setServerAddr();
    bindServer();

    std::cout << "Server created with default values" << std::endl;
}

Server::Server(std::string name, std::string password, std::string port) : _name(name), _password(password), _port(port), _serverSocket(0) {
    setServerSocket();
    setServerAddr();
    bindServer();

    std::cout << "Server created with custom values" << std::endl;
}


Server::~Server() {
    close(_serverSocket);
    std::cout << "Server destroyed" << std::endl;
}

std::string Server::getName() {return this->_name;}
std::string Server::getPassword() {return this->_password;}

void Server::setServerSocket() {
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        // close(this->_serverSocket);

    if (this->_serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }
}

void Server::setServerAddr() {

    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
    this->_serverAddr.sin_family = AF_INET;
    this->_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->_serverAddr.sin_port = htons(std::stoi(this->_port));   
}

void Server::bindServer() {
        // close(this->_serverSocket);

    if (bind(this->_serverSocket, reinterpret_cast<sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(this->_serverSocket);
        exit(1);
    }
    std::cout << "Server binded to port " << this->_port << std::endl;
}

void Server::listenServer() {

    if (listen(this->_serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(this->_serverSocket);
        exit(1);
    }
    std::cout << "Server listening on port " << this->_port << std::endl;
}

int const Server::getServerSocket() {return this->_serverSocket;}