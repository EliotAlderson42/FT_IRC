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
    std::cout << "str = "<< str<<std::endl;
    (void)delimiter;
    while (std::getline(ss, command, delimiter)) {
        std::cout << "command == " << command << std::endl;
        commands.push_back(command);
        ss.clear();
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