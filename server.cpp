#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 8080;
const int MAX_CONNECTIONS = 5;
const int BUFFER_SIZE = 1024;
int running = 1;

void signalHandler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "Arret du serveur...\n";
        running = 0;
    }
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Erreur lors de la creation du socket.\n";
        return (0);
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8080);
    // close(serverSocket);
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        std::cerr << "Erreur lors de la liaison du socket \n";
        close(serverSocket);
        return (0);
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Erreur lors de la mise en ecoute du socket.\n";
        close(serverSocket);
        return (0);
    }

    std::cout << "Serveur en ecoute sur le port " << PORT << std::endl;
    // signal(SIGINT, signalHandler);
    while (running == 1)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Erreur lors de l'acceptation de la connexion.\n";
            continue ;
        }

        std::cout << "Nouvelle connexion accepter.\n";

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) 
        {
            send(clientSocket, buffer, bytesRead, 0);
        }

        close(clientSocket);
    }
    close(serverSocket);

    return (0);
}