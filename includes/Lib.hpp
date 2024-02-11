#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <vector>
#include <sstream>
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

#define RESET   "\033[0m"
#define RED     "\033[31m"     
#define GREEN   "\033[32m"     
#define YELLOW  "\033[33m"     
#define BLUE    "\033[34m"     
#define MAGENTA "\033[35m"     
#define CYAN    "\033[36m"     
#define WHITE   "\033[37m" 

const int BUFFER_SIZE = 1024;
extern int running;

void NICK(std::string receivedData, int i, Client *client);
void handlePassword(Server *server, int clientSocket, sockaddr_in clientAddr, std::unordered_map<int, Client *> &clients);
void handleChannelPassword(std::unordered_map<int, Client *> &clients, int fd, std::string &receivedData, Channel *channel);
std::string firstWord(std::string &phrase);
