#include "Lib.hpp"

void Server::quit(std::string str, int socket)
{
    std::string word = str.substr(6);
    _clients[socket]->diffuseMessage("Quit: " + word);
    _clients[socket]->removeAllChannels();
    close(socket);
    delete _clients[socket];
    _clients.erase(socket);
}
