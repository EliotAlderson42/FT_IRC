#include "Lib.hpp"



int main()
{
    Server *server = new Server("default", "default", "8080");
    Client clicli("default", "default", "default", server);
    server->listenServer();
    clicli.runClient();
    close(server->getServerSocket());
    delete server;
    return (0);
}