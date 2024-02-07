#include "Lib.hpp"


const int BUFFER_SIZE = 1024;
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
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        return -1;
    }
    return 0;
}

int main()
{
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
    Server *server = new Server("default", "default", "8080");
    std::unordered_map<int, Client *> clients(10);
    int eventSize = 1;
    server->listenServer();
    
    int epollFd = epoll_create1(0); // je cree l'instance epoll
    if (epollFd == -1)
    {
        std::cerr << "Erreur lors de la creation de l'instance epoll"  << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<struct epoll_event> events(10);// je cree la structure de gestion des events
    events[0].events = EPOLLIN ; //EPOLLIN indique que l'evenement sera quelque chose a lire
    events[0].data.fd = server->getServerSocket(); // je recupere le socket 
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, server->getServerSocket(), &events[0]) == -1) // je lie l'instance epoll avec le socket et la structure de gestion des events
    {
        std::cerr << "Erreur lors de l'ajout du socket server a l'instance epoll"  << std::endl;
        return EXIT_FAILURE;
    }
    
    while (running == 1)
    {
        int numEvents = epoll_wait(epollFd, events.data(), events.size(), -1);
        if (numEvents == -1)
        {
            if (errno == EINTR) 
                continue; // Le signal a interrompu l'appel à epoll_wait(), réessayer
            std::cerr << "Erreur lors de l'appel à epoll_wait()."  << std::endl;
            break;
        }

        if (static_cast<size_t>(numEvents) > events.size())
            events.resize(numEvents);

        for (int i = 0; i < numEvents; i++)
        {
            if (events[i].data.fd == server->getServerSocket())
            {
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
                    std::cerr << "Erreur lors de l'ajout du socket client à l'instance epoll." << std::endl;
                    close(clientSocket);
                    continue;
                }
                clients[clientSocket] = new Client(clientSocket, clientAddr, server);
            }
            else
            {
                if (setNonBlocking(events[i].data.fd) == -1) {
                    std::cerr << "Erreur lors de la mise en mode non bloquant du socket." << std::endl;
                    continue;
                }
                char buffer[BUFFER_SIZE];
                ssize_t bytesRead;
                while ((bytesRead = recv(events[i].data.fd, buffer, sizeof(buffer), 0)) > 0) 
                {
                    std::string receivedData(buffer, bytesRead);

                    NICK(receivedData, events[i].data.fd, clients[events[i].data.fd]);

                    if (receivedData.find("JOIN") != std::string::npos)
                    {
                        if (channel->findClient(events[i].data.fd))
                            send(events[i].data.fd, "You are already in a channel", 27, 0);
                        else if (clients[events[i].data.fd]->getNickname().empty())
                            send(events[i].data.fd, "You need a nickname to enter that channel.", 41, 0);
                        else
                            channel->inviteClient(clients[events[i].data.fd]);
                    }
                    
                    else if (channel->findClient(events[i].data.fd))
                    {
                        if (!clients[events[i].data.fd]->getNickname().empty())
                        {
                            receivedData.insert(0, clients[events[i].data.fd]->getNickname() + " : ");
                            receivedData.erase(std::remove(receivedData.begin(), receivedData.end(), '\n'), receivedData.end());
                        }
                        channel->diffuseMessage(receivedData);
                    }
                }
            }
        }
    }
    close(server->getServerSocket());

    delete server;
    return (0);
}