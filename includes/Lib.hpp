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
#include <map>
#include <algorithm>
#include <cerrno>
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

std::string toString(int value);
void NICK(std::string receivedData, int i, Client *client);
std::string firstWord(std::string &phrase);
std::vector<std::string> splitCommands(const std::string& str);

                                    //             TO_DO LIST:

                                    // 1 : Commande KICK;

                                    // 2 : Commande INVITE;

                                    // 3 : Commande TOPIC;

                                    // 4 : Commande QUIT;

                                    // 5 : Commande LIST;

                                    // 6 : Commande MODE : [flags]
                                    //                     -/+ i;
                                    //                     -/+ t;
                                    //                     -/+ k;
                                    //                     -/+ o;
                                    //                     -/+ l;