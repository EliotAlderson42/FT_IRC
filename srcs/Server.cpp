#include "Lib.hpp"

Server::Server() : _name("localhost"), _password("default"), _port("6667"), _serverSocket(0) {
    setServerSocket();
    setServerAddr();
    bindServer();

    std::cout << "Server created with default values" << std::endl;
}

Server::Server(std::string name, std::string password, std::string port) : _name(name), _password(password), _port(port), _serverSocket(0) {
    setServerSocket();
    setServerAddr();
    bindServer();
    // _funcTab.resize(3);
    _funcTab["JOIN"] = &Server::join;
	_funcTab["PRIVMSG"] = &Server::privmsg;
	_funcTab["PING"] = &Server::pong;
	_funcTab["WHOIS"] = &Server::whois;
    _funcTab["CAP"] = &Server::capls;
    _funcTab["PASS"] = &Server::pass;
    _funcTab["NICK"] = &Server::nick;
    _funcTab["USER"] = &Server::user;
    _funcTab["QUIT"] = &Server::quit;

    std::cout << "Server created with custom values" << std::endl;
}

Server::~Server() {
    close(_serverSocket);
    std::cout << "Server destroyed" << std::endl;
}

std::string Server::getName() {return this->_name;}
std::string Server::getPassword() {return this->_password;}

void Server::setServerSocket() {
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        // close(this->_serverSocket);

    if (this->_serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }
}

void Server::setServerAddr() {

    int port;
    std::istringstream(this->_port) >> port;
    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
    this->_serverAddr.sin_family = AF_INET;
    this->_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->_serverAddr.sin_port = htons(port);   
}

void Server::bindServer() {
        // close(this->_serverSocket);

    if (bind(this->_serverSocket, reinterpret_cast<sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(this->_serverSocket);
        exit(1);
    }
    std::cout << "Server binded to port " << this->_port << std::endl;
}

void Server::listenServer() {

    if (listen(this->_serverSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(this->_serverSocket);
        exit(1);
    }
    std::cout << "Server listening on port " << this->_port << std::endl;
}

int Server::initServ()
{
    this->listenServer();
    this->_events.resize(10);
    this->_epollFd = epoll_create1(0);
    if (this->_epollFd == -1)
    {
        std::cerr << "Error creating epoll instance\n";
        return (0);
    }
    this->_events[0].events = EPOLLIN;
    this->_events[0].data.fd = this->getServerSocket();
    if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->getServerSocket(), this->_events.data()) == -1) 
    {
        std::cerr << "Error adding socket server to epoll instance"  << std::endl;
        return 0;
    }
    return (1);
}

int Server::addClient()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(this->getServerSocket(), reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientSocket == -1)
    {
        std::cerr << "Error : connection refused\n";
        return (0);
    }
    std::cout << "New client connection accepted.\n";
    struct epoll_event clientEvent;
    clientEvent.events = EPOLLIN | EPOLLET; // Surveiller les événements de lecture
    clientEvent.data.fd = clientSocket;
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1)
    {   
        std::cerr << "Error adding client socket to epoll instance." << std::endl;
        close(clientSocket);
        return (0);
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
        std::cerr << "Error setting socket to non-blocking mode.\n";
    this->_clients[clientSocket] = new Client(clientSocket, clientAddr);
    std::string msg = RPL_WELCOME(_clients[clientSocket]->getNickname());
    send(clientSocket, msg.c_str(), msg.size(), 0);
    return (1);
}

void Server::removeClient(int socket)
{
    close(socket);
    _clients.erase(socket);
}

int    Server::getPassLength()
{
    return (this->_password.size());
}



void Server::join(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;

    if (word[0] == '#')
    {
        std::cout << word << std::endl;
		if (_channels.find(word) != _channels.end())
        {
           _channels[word]->addSocket(socket);
           _channels[word]->setOperator(socket);
           _clients[socket]->setIsOperator(1);
           std::string msg = RPL_JOIN(_clients[socket]->getNickname(), word);
           send(socket, msg.c_str(), msg.size(), 0);
        }
        else
        {
            _channels[word] = new Channel(word);
            _channels[word]->addSocket(socket);
			std::string msg = RPL_JOIN(_clients[socket]->getNickname(), word);
    		send(socket, msg.c_str(), msg.size(), 0);
       }
    }
}

void	Server::pong(std::string str, int socket)
{
	(void)str;
	(void)socket;
	std::string msg = RPL_PONG;
	send(socket, msg.c_str(), msg.size(), 0);
}

void Server::privmsg(std::string str, int socket)
{
	std::istringstream iss(str);
	std::string word;
	iss >> word;
	iss >> word;

	std::cout << "channel = " << word << std::endl;
	std::string toSend;
    size_t pos = str.find(':');
    if (pos != std::string::npos)
        toSend = str.substr(pos);
    else
    {
        std::string msg = ERR_NOTEXTTOSEND(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
	if (word[0] == '#')
	{
    	std::string msg = RPL_PRIVMSG_CHANNEL((CYAN + _clients[socket]->getNickname() + RESET), word, toSend);
        std::cout << _clients[socket]->getNickname() << std::endl;
		_channels[word]->sendChanMsg(socket, msg);
        return ;
	}
	else
	{
        for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            if (it->second->getNickname() == word)
            {
                // std::string msg = RPL_PRIVMSG_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), it->second->getNickname(), toSend);
    	        std::string msg = RPL_PRIVMSG_CHANNEL((RED +_clients[socket]->getNickname() + RESET), ("#" + word), toSend);
                // std::string msg = (RED + _clients[socket]->getNickname() + RESET) + toSend;
                std::cout << msg << std::endl;
                send(it->second->getSocket(), msg.c_str(), msg.size(), 0);
                return ;
            }
        }
	}
    std::string msg = _clients[socket]->getNickname() + " :No user with this name\r\n";
    send(socket, msg.c_str(), msg.size(), 0);
}

void    Server::whois(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    std::cout << "word = " << word << std::endl;
    if (word == _clients[socket]->getNickname())
    {
        std::string msg = RPL_WHOISUSER(_clients[socket]->getNickname(), this->getName(), _clients[socket]->getUsername());
        send(socket, msg.c_str(), msg.size(), 0);
        msg = RPL_ENDOFWHOIS(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname() , word);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}

void Server::nick(std::string str, int socket)
{
    std::string sub = str.substr(5, 6);
    std::cout << "SUB = "<< sub << std::endl;
    if (sub.empty())
    {
        std::string msg = ERR_NONICKNAMEGIVEN(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else if (sub.find("#") != std::string::npos || sub.find(":") != std::string::npos || sub.find(" ") != std::string::npos || sub.find("\t") != std::string::npos || sub.find("\r") != std::string::npos || sub.find("\n") != std::string::npos || sub.find("\v") != std::string::npos || sub.find("\f") != std::string::npos)
    {
        std::string msg = ERR_ERRONEUSNICKNAME(sub);
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            if (it->second->getNickname() == sub)
            {
                std::string msg = ERR_NICKNAMEINUSE(sub);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
        }
        _clients[socket]->setNickname(sub);
        // std::string msg = RPL_WELCOME(_clients[socket]->getNickname());
        // send(socket, msg.c_str(), msg.size(), 0);
        std::string msg = ":localhost 001 " + sub + " :Nickname changed to " + sub + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
    }
}


void Server::pass(std::string str, int socket)
{
    std::string pass = str.substr(5, this->getPassLength());
    // pass = str.substr(0, 4);
    std::cout << "pass = " << pass << std::endl;
    if (pass == this->getPassword())
    {
        std::string msg = "Correct Password !\n";
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(1);
    }
    else
    {
        std::cout << "salut les zgeg\n";
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(0);
        // removeClient(socket);
    }
}

void Server::user(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    if (_clients[socket]->getExpectingUsername() == 0)
    {
        std::string msg = ERR_ALREADYREGISTRED(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else if (word.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "USER");
        send(socket, msg.c_str(), msg.size(), 0);
    }
    else
    {
        _clients[socket]->setUsername(word);
        std::string msg = "Username changed to " + word + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
    }
}

void Server::quit(std::string str, int socket)
{
    (void)str;
    close(socket);
    _clients.erase(socket);
}

void Server::capls(std::string str, int socket)
{
   std::vector<std::string> commands = splitCommands(str);
   std::vector<std::string>::iterator it = commands.begin();
   int pass = 0;
   
   if (it != commands.end()) // Check to avoid incrementing an end iterator
       ++it;

   for(; it != commands.end(); it++)
   {
        std::string word = *it;
        std::cout << "word = " << word << std::endl; 
        if (word.substr(0, 4) == "PASS")
            pass = 1;
       if (word.substr(0, 4) == "MODE" || word.substr(0, 5) == "WHOIS")
            continue ;
       (this->*_funcTab[word.substr(0,4)])(word,socket);
   }
    std::cout << "isConnect = " << _clients[socket]->getIsConnect() << std::endl;
   if (_clients[socket]->getIsConnect() == 0)
    {
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        close(socket);
        _clients.erase(socket);
        return ;
    }
    // std::string msg = RPL_WELCOME(_clients[socket]->getNickname());
    // send(socket, msg.c_str(), msg.size(), 0);
}

void Server::mainLoop()
{
    while (running == 1)
    {
        this->_numEvents = epoll_wait(this->_epollFd, this->_events.data(), 10, -1);
        std::cout << "numevents = " << this->_numEvents << std::endl;
        if (this->_numEvents == -1)
        {
            if (errno == EINTR)
                continue ;
            std::cerr << "Error calling epoll_wait().\n";
            break ;
        }
        if (static_cast<size_t>(_numEvents) > _events.size())
            _events.resize(_numEvents);
        for (int i = 0; i < _numEvents; i++)
        {
            if (_events[i].data.fd == this->getServerSocket())
                this->addClient();
            else
            {
                    char buffer[1024] = {0};
                    ssize_t bytesRead;
                    bytesRead = recv(_events[i].data.fd, buffer, sizeof(buffer), 0);
                    std::cout << "\033[35m"<< "Receive : " << buffer << "\033[0m" << std::endl;
                    std::string neww(buffer);
                    if (this->_funcTab.find(firstWord(neww)) != this->_funcTab.end())
                    {
                        std::cout << "LE BUFFER : " << neww << std::endl;
                        std::string neww(buffer);
                        (this->*_funcTab[firstWord(neww)])(neww, _events[i].data.fd);
                    }
                    memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}

int const Server::getServerSocket() {return this->_serverSocket;}