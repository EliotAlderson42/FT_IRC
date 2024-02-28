#include "Lib.hpp"
class Server;
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
    if (argc < 2)
        return (std::cout << "pas le bon nombre d'arguments\n", 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signalHandler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        std::cerr << "Error: cannot handle SIGINT" << std::endl;
        return EXIT_FAILURE;
    }
    // Channel *channel = new Channel("default", "default");
    Server *server = new Server("default", argv[2], argv[1]);
    server->initServ();
    server->mainLoop();
    close(server->getServerSocket());
    delete server;
    return (0);
}
