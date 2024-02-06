#include "Client.hpp"

const int BUFFER_SIZE = 1024;
int running = 1;

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "Server stoppage..." << std::endl;
        running = 0;
    }
}

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

void Client::runClient() {
    socklen_t clientAddrLen = sizeof(this->_clientAddr);
    std::cout << "adress len : " << clientAddrLen << std::endl;
    while (running == 1)
    {
        int clientSocket = accept(this->_clientServer->getServerSocket(), reinterpret_cast<sockaddr*>(&this->_clientAddr), &clientAddrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Error : connection refused." << std::endl;
            break ;
        }
        else
            std::cout << "New client's connection accepted." << std::endl;

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) 
        {
            std::cout << bytesRead << std::endl;
            send(clientSocket, buffer, bytesRead, 0);
        }

        close(clientSocket);
    }
    std::cout << "Client running" << std::endl;
}