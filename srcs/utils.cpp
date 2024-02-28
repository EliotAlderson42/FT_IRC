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

std::vector<std::string> splitCommands(const std::string& str, char delimiter) {
    std::vector<std::string> commands;
    std::istringstream ss(str);
    std::string command;

    while (std::getline(ss, command, delimiter)) {
        commands.push_back(command);
    }

    return commands;
}

bool onlyWhitespace(const std::string &str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        char c = *it;
        std::cout << "Char: " << c << ", ASCII: " << static_cast<int>(c) << std::endl;
        if (!std::isspace(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}

// void signalHandler(int signum, siginfo_t *info, void *ptr)
// {
//     if (signum == SIGINT)
//     {
//         std::cout << "\nServer stoppage..." << std::endl;
//         running = 0;
//     }
// }