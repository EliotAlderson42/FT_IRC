#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <vector>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <algorithm>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "RPL.hpp"

const int BUFFER_SIZE = 1024;
extern int running;

void NICK(std::string receivedData, int i, Client *client);
void handlePassword(Server *server, int clientSocket, sockaddr_in clientAddr, std::unordered_map<int, Client *> &clients);
void handleChannelPassword(std::unordered_map<int, Client *> &clients, int fd, std::string &receivedData, Channel *channel);
