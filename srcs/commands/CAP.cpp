#include "Lib.hpp"

void Server::capls(std::string str, int socket)
{
   std::vector<std::string> commands = splitCommands(str, '\n');
   std::vector<std::string>::iterator it = commands.begin();

   if (it != commands.end())
       ++it;

   for(; it != commands.end(); it++)
   {
        std::string word = *it;
        (this->*_funcTab[word.substr(0,4)])(word,socket);
   }
   if (_clients[socket]->getIsConnect() == 0)
    {
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        close(socket);
        _clients.erase(socket);
        return ;
    }
}