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
    Server *server = new Server("default", "default", "8080");
    std::unordered_map<int, Client> clients(10);
    int eventSize = 1;
    server->listenServer();
    
    int epollFd = epoll_create1(0); // je cree l'instance epoll
    if (epollFd == -1)
    {
        std::cerr << "Erreur lors de la creation de l'instance epoll\n";
        return EXIT_FAILURE;
    }

    std::vector<struct epoll_event> events(10);// je cree la structure de gestion des events
    events[0].events = EPOLLIN ; //EPOLLIN indique que l'evenement sera quelque chose a lire
    events[0].data.fd = server->getServerSocket(); // je recupere le socket 
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, server->getServerSocket(), &events[0]) == -1) // je lie l'instance epoll avec le socket et la structure de gestion des events
    {
        std::cerr << "Erreur lors de l'ajout du socket server a l'instance epoll\n";
        return EXIT_FAILURE;
    }
    
    while (running == 1)
    {
        int numEvents = epoll_wait(epollFd, events.data(), events.size(), -1);
        if (numEvents == -1)
        {
            if (errno == EINTR) 
                continue; // Le signal a interrompu l'appel à epoll_wait(), réessayer
            std::cerr << "Erreur lors de l'appel à epoll_wait().\n";
            break;
        }

        if (static_cast<size_t>(numEvents) > events.size())
            events.resize(numEvents); 

        for (int i = 0; i < numEvents; i++)
        {
            if (events[i].data.fd == server->getServerSocket())
            {
                sockaddr_in clientAddr;
                // memset(&clientAddr, 0, sizeof(clientAddr));
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
                    std::cerr << "Erreur lors de l'ajout du socket client à l'instance epoll.\n";
                    close(clientSocket);
                    continue;
                }
                clients[clientSocket] = Client(clientSocket, clientAddr, server);
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
                    std::cout << bytesRead << std::endl;
                    send(events[i].data.fd, buffer, bytesRead, 0);
                }
            }
        }
        // close(clientSocket);
    }
    close(server->getServerSocket());

    delete server;
    return (0);
}