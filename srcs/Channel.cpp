#include "Lib.hpp"

Channel::Channel() : _name("default"), _password("") {
    std::cout << "Channel created with default values" << std::endl;
}

Channel::Channel(std::string name) : _name(name){
    std::cout << "Channel created with custom values" << std::endl;
}

Channel::~Channel() {
    std::cout << "Channel destroyed" << std::endl;
}

void    Channel::addSocket(int socket)
{
    this->_sockets.push_back(socket);
}

// void    Channel::sendGrpMsg(std::string str)
// {
//     std::string toSend;
//     size_t pos = str.find(':');
//     if (pos != std::string::npos)
//         toSend = str.substr(pos);
//     // else
//     // {
//     //     \\\\\\
//     //     \\\\\\
//     //     \\\\\\
//     // }
//     std::string msg = RPL_PRIVMSG_CHANNEL()
// }

// void Channel::inviteClient(Client *client) {
//     _clients[client->getSocket()] = client;
//     std::cout << "Client invited to channel" << std::endl;
// }

// void Channel::diffuseMessage(std::string message) {
//     for (auto it = _clients.begin(); it != _clients.end(); it++) {
//         message.append("\n");
//         send(it->first, message.c_str(), message.size(), 0);
//     }
// }

std::string Channel::getPassword() const {return this->_password;}

// Client *Channel::findClient(int clientSocket) {
//     return _clients[clientSocket];
// }