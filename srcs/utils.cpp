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
    // std::istringstream ss(str);
    std::string neww;
    std::string command;
    size_t lastChar = str.find_last_not_of(" \n\r\t");
    std::cout << "la string = " << str << std::endl;
    if(lastChar != std::string::npos)
        neww = str.substr(0, lastChar + 1);
    std::istringstream ss(neww);
    // (void)delimiter;
    while (std::getline(ss, command, delimiter)) {
        commands.push_back(command);
        ss.clear();
    }
    return commands;
}

bool onlyWhitespace(const std::string &str) {
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        char c = *it;
        if (!std::isspace(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}