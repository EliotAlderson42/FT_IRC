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

void NICK(std::string receivedData, int i, Client *client);
