#include "Lib.hpp"
class Server;
int running = 1;

void signalHandler(int signum, siginfo_t *info, void *ptr)
{
    (void)info;
    (void)ptr;
    if (signum == SIGINT)
    {
        std::cout << "\nServer stoppage..." << std::endl;
        running = 0;
    }
}

int setNonBlocking(int sockfd) 
{
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
        return (std::cout << "Usage : ircserv : [port] [Password]\n", 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signalHandler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        std::cerr << "Error: cannot handle SIGINT" << std::endl;
        return EXIT_FAILURE;
    }
    std::string mdp; //= (argc == 3) ? argv[2] : NULL;
    if (argc == 2)
        mdp = "";
    else 
        mdp = argv[2];
    Server *server = new Server("default", mdp, argv[1]);
    server->setIsPasswd(1);
    if (argc == 2)
        server->setIsPasswd(0);
    server->initServ();
    server->mainLoop();
    close(server->getServerSocket());
    delete server;
    return (0);
}
