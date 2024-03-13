#include "Lib.hpp"

Server::Server() : _name("localhost"), _password("default"), _port("6667"), _serverSocket(0) {
    setServerSocket();
    setServerAddr();
    bindServer();
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
    _funcTab["TOPIC"] = &Server::topic;
    _funcTab["KICK"] = &Server::kick;
    _funcTab["INVITE"] = &Server::invite;
    _funcTab["LIST"] = &Server::list;
    _funcTab["MODE"] = &Server::mode;
    _modes["o"] = &Server::oMode;
    _modes["i"] = &Server::iMode;
    _modes["t"] = &Server::tMode;
    _modes["k"] = &Server::kMode;
    _modes["l"] = &Server::lMode;
}

Server::~Server() {
    close(_serverSocket);
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        close(it->first);
        delete it->second;
    }
    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        delete it->second;
    }
    std::cout << "Server destroyed" << std::endl;
}

std::string Server::getName() {return this->_name;}
std::string Server::getPassword() {return this->_password;}
std::map<int, Client *> Server::getClients() {return this->_clients;}
std::map<std::string, Channel *> Server::getChannels() {return this->_channels;}

void Server::setServerSocket() 
{
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
    }
}

int Server::getClientSocket(std::string nickname)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return it->first;
    }
    return 0;
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
    int enable = 1;
    if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(this->_serverSocket);
        exit(1);
    }
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

Channel *Server::getChannel(std::string name)
{
    if (_channels.find(name) != _channels.end())
        return _channels[name];
    return NULL;
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

void	Server::pong(std::string str, int socket)
{
	(void)str;
	(void)socket;
	std::string msg = RPL_PONG;
	send(socket, msg.c_str(), msg.size(), 0);
}


int Server::getServerSocket() {return this->_serverSocket;}


void Server::mainLoop()
{
    while (running == 1)
    {
        this->_numEvents = epoll_wait(this->_epollFd, this->_events.data(), 10, -1);
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
                    recv(_events[i].data.fd, buffer, sizeof(buffer), 0);
                    std::cout << "\033[35m"<< "Receive : " << buffer << "\033[0m" << std::endl;
                    std::string neww(buffer);
                    if (neww.find('\x04') != std::string::npos)
                        continue ;
                    if (this->_funcTab.find(firstWord(neww)) != this->_funcTab.end())
                    {
                        // std::string neww(buffer);
                        (this->*_funcTab[firstWord(neww)])(neww, _events[i].data.fd);
                    }
                    memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}