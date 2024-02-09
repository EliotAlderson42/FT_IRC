#include "Lib.hpp"

int running = 1;
void signalHandler(int signum, siginfo_t *info, void *ptr)
{
    if (signum == SIGINT)
    {
        std::cout << "\nServer stoppage..." << std::endl;
        running = 0;
    }
}

int setNonBlocking(int sockfd) {
    // int flags = fcntl(sockfd, F_GETFL, 0);
    // if (flags == -1) {
    //     return -1;
    // }
    // flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (0);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signalHandler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        std::cerr << "Error: cannot handle SIGINT" << std::endl;
        return EXIT_FAILURE;
    }
    Channel *channel = new Channel("default", "default");
    Server *server = new Server("default", "default", argv[1]);
    std::unordered_map<int, Client *> clients(10);
    int eventSize = 1;
    server->listenServer();
    // close(server->getServerSocket());
    int epollFd = epoll_create1(0); // je cree l'instance epoll
    if (epollFd == -1)
    {
        std::cerr << "Error creating epoll instance"  << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<struct epoll_event> events(10);// je cree la structure de gestion des events
    events[0].events = EPOLLIN ; //EPOLLIN indique que l'evenement sera quelque chose a lire
    events[0].data.fd = server->getServerSocket(); // je recupere le socket 
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, server->getServerSocket(), &events[0]) == -1) // je lie l'instance epoll avec le socket et la structure de gestion des events
    {
        std::cerr << "Error adding socket server to epoll instance"  << std::endl;
        return EXIT_FAILURE;
    }
    int numEvents;
    while ( running == 1)
    {
        std::cout << "size events " << events.size() << std::endl; 
        numEvents = epoll_wait(epollFd, events.data(), 1, -1);
        std::cout << "numEvents [" << numEvents << "]\n";
        if (numEvents == -1)
        {
            if (errno == EINTR)
                continue; // Le signal a interrompu l'appel à epoll_wait(), réessayer
            std::cerr << "Error calling epoll_wait()."  << std::endl;
            break;
        }

        // if (static_cast<size_t>(numEvents) > events.size())
        //     events.resize(numEvents);

        for (int i = 0; i < numEvents; i++)
        {
            if (events[i].data.fd == server->getServerSocket())
            {
                std::cout << "events[i].data.fd [" << events[i].data.fd << "]\n";
                sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(server->getServerSocket(), reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
                if (clientSocket == -1)
                {
                    std::cerr << "Error : connection refused." << std::endl;
                    continue ;
                }
                else
                    std::cout << "New client's connection accepted." << std::endl;

                struct epoll_event clientEvent;
                clientEvent.events = EPOLLIN | EPOLLET; // Surveiller les événements de lecture
                clientEvent.data.fd = clientSocket;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1)
                {   
                    std::cerr << "Error adding client socket to epoll instance." << std::endl;
                    close(clientSocket);
                    continue;
                }
                // if (!server->getPassword().empty())
                // {
                //     handlePassword(server, clientSocket, clientAddr, clients);
                // }
                // else
                // {
                // send(clientSocket, "Welcome to the server !\n", 24, 0);
                clients[clientSocket] = new Client(clientSocket, clientAddr, server);
                // }
                if (setNonBlocking(clientSocket) == -1) {
                    std::cerr << "Error setting socket to non-blocking mode." << std::endl;
                    continue;
                }
                std::string name = "yahouariqqqqqqq";
                std::string msg = RPL_WELCOME(name);
                send(clientSocket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                    char buffer[1024] = {0};
                    ssize_t bytesRead;
                    bytesRead = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                    std::cout << "bytesread " << bytesRead << std::endl;
                    std::cout << "buffer " << buffer << std::endl;
                    memset(buffer, 0, sizeof(buffer));
                    // send(server->getServerSocket(), buffer, sizeof(buffer), 0);
                    // std::string receivedData(buffer, bytesRead);

                    // NICK(receivedData, events[i].data.fd, clients[events[i].data.fd]);

                    // if (receivedData.find("JOIN") != std::string::npos)
                    // {
                    //     if (channel->findClient(events[i].data.fd))
                    //         send(events[i].data.fd, "You are already in a channel.\n", 30, 0);
                    //     else if (clients[events[i].data.fd]->getNickname().empty())
                    //         send(events[i].data.fd, "You need a nickname to enter that channel.\n", 43, 0);
                    //     else if (!channel->getPassword().empty())
                    //     {
                    //         send(events[i].data.fd, "Enter the channel's password : ", 31, 0);
                    //         clients[events[i].data.fd]->isExpectingPassword(true);
                    //     }
                    //     else
                    //     {
                    //         send(events[i].data.fd, "Channel joined !\n", 18, 0);
                    //         channel->inviteClient(clients[events[i].data.fd]);
                    //     }
                    // }
                    // else if (clients[events[i].data.fd]->getExpectingPassword())
                    //     handleChannelPassword(clients, events[i].data.fd, receivedData, channel);
                    // else if (channel->findClient(events[i].data.fd))
                    // {
                    //     if (!clients[events[i].data.fd]->getNickname().empty())
                    //     {
                    //         receivedData.insert(0, clients[events[i].data.fd]->getNickname() + " : ");
                    //         receivedData.erase(std::remove(receivedData.begin(), receivedData.end(), '\n'), receivedData.end());
                    //     }
                    //     channel->diffuseMessage(receivedData);
                    // }
            }
        }
    }
    close(server->getServerSocket());

    delete server;
    return (0);
}