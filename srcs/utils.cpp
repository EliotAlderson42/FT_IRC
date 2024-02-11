#include "Lib.hpp"

std::string firstWord(std::string &phrase) {
    std::istringstream iss(phrase);
    std::string premier_mot;
    iss >> premier_mot;
    return premier_mot;
}

// void signalHandler(int signum, siginfo_t *info, void *ptr)
// {
//     if (signum == SIGINT)
//     {
//         std::cout << "\nServer stoppage..." << std::endl;
//         running = 0;
//     }
// }