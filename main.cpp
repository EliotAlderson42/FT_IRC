#include "Server.hpp"

const int BUFFER_SIZE = 1024;
int running = 1;

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "Server stoppage...\n";
        running = 0;
    }
}

int main()
{
    Server server("test", "test", "8080");
    server.listenServer();
    while (running == 1)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(server.getServerSocket(), reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Error : connection refused.\n";
            continue ;
        }
        else
            std::cout << "New client's connection accepted.\n";

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) 
        {
            send(clientSocket, buffer, bytesRead, 0);
        }

        close(clientSocket);
    }
    close(server.getServerSocket());

    return (0);
}