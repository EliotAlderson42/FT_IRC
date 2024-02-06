#include "Lib.hpp"


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

int main()
{
    Server *server = new Server("default", "default", "8080");
    Client clicli("default", "default", "default", server);
    server->listenServer();

    while (running == 1)
    {
        sockaddr_in clientAddr = clicli.getClientAddr();
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(server->getServerSocket(), reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Error : connection refused." << std::endl;
            continue ;
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
    close(server->getServerSocket());

    delete server;
    return (0);
}