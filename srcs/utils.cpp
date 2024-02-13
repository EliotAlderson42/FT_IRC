#include "Lib.hpp"

std::string toString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string firstWord(std::string &phrase) {
    std::istringstream iss(phrase);
    std::string premier_mot;
    iss >> premier_mot;
    return premier_mot;
}

std::vector<std::string> splitCommands(const std::string& str) {
    std::vector<std::string> commands;
    std::istringstream ss(str);
    std::string command;

    while (std::getline(ss, command, '\n')) {
        commands.push_back(command);
    }

    return commands;
}

// void signalHandler(int signum, siginfo_t *info, void *ptr)
// {
//     if (signum == SIGINT)
//     {
//         std::cout << "\nServer stoppage..." << std::endl;
//         running = 0;
//     }
// }