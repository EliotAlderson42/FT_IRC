#include "Lib.hpp"

void NICK(std::string receivedData, int i, Client *clients)
{
    if (receivedData.find("NICK") != std::string::npos)
    {
        send(i, "Choose your nickname : ", 23, 0);
        clients->isExpectingNickname(true);
    }
    else if (clients->getExpectingNickname())
    {
        clients->setNickname(receivedData);
        clients->isExpectingNickname(false);
        send(i, "Nickname set.\n", 14, 0);
    }
}