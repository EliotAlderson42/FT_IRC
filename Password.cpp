#include "Lib.hpp"

void handlePassword(Server *server, int clientSocket, sockaddr_in clientAddr, std::unordered_map<int, Client *> &clients)
{
    if (!server->getPassword().empty())
    {
        send(clientSocket, "Enter the server's password : ", 30, 0);
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        std::string receivedData(buffer, bytesRead);
        receivedData.erase(std::remove(receivedData.begin(), receivedData.end(), '\n'), receivedData.end());
        std::cout <<  "Received password : " << receivedData << std::endl;
        std::cout << "Server's password : " << server->getPassword() << std::endl;
        if (receivedData != server->getPassword())
        {
            send(clientSocket, "Wrong password :(\n", 19, 0);
            close(clientSocket);
        }
        else
        {
            send(clientSocket, "Welcome to the server !\n", 24, 0);
            clients[clientSocket] = new Client(clientSocket, clientAddr, server);
        }
    }
    else
    {
        send(clientSocket, "Welcome to the server !\n", 24, 0);
        clients[clientSocket] = new Client(clientSocket, clientAddr, server);
    }
}

void handleChannelPassword(std::unordered_map<int, Client *> &clients, int fd, std::string &receivedData, Channel *channel)
{
    if (clients[fd]->getExpectingPassword())
    {
        receivedData.erase(std::remove(receivedData.begin(), receivedData.end(), '\n'), receivedData.end());
        if (receivedData != channel->getPassword())
        {
            send(fd, "Wrong password :(\n", 19, 0);
            clients[fd]->isExpectingPassword(false);
        }
        else
        {
            send(fd, "Channel joined !\n", 18, 0);
            clients[fd]->isExpectingPassword(false);
            channel->inviteClient(clients[fd]);
        }
    }
}