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

void Server::oMode(bool mode, std::string channel, std::string nickname, int socket)
{
    if (nickname.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (_clients.find(getClientSocket(nickname)) == _clients.end())
    {
        std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname(), nickname);
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    else if (!_channels[channel]->isInChannel(getClientSocket(nickname)))
    {
        std::string msg = ERR_NOTONCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_NEWOPER(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "o", nickname)
                           : UNSET_OPER(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "o", nickname);
    mode ? _channels[channel]->setOperator(getClientSocket(nickname)) 
         : _channels[channel]->removeOperator(getClientSocket(nickname));
    _channels[channel]->diffuseMessage(msg);
}

void Server::iMode(bool mode, std::string channel, std::string nickname, int socket)
{
    (void)nickname;
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "i")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "i");
    _channels[channel]->setInviteOnly(mode);
    _channels[channel]->diffuseMessage(msg);
}

void Server::tMode(bool mode, std::string channel, std::string nickname, int socket)
{
    (void)nickname;
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "t")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "t");
    _channels[channel]->setRestrictedTopic(mode);
    _channels[channel]->diffuseMessage(msg);
}

void Server::kMode(bool mode, std::string channel, std::string password, int socket)
{
    if (password.empty())
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "k")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "k");
    mode ? _channels[channel]->setPassword(password)
         : _channels[channel]->setPassword("");
    _channels[channel]->diffuseMessage(msg);
}

void Server::lMode(bool mode, std::string channel, std::string limit, int socket)
{
    int lim = atoi(limit.c_str());
    if (mode && !lim)
    {
        std::string msg = ERR_NEEDMOREPARAMS(_clients[socket]->getNickname(), "MODE");
        send(socket, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::string msg = mode ? SET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "l")
                           : UNSET_channel_MODE(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "MODE", channel.substr(1), "l");
                      mode ? _channels[channel]->setLimit(lim)
                           : _channels[channel]->setLimit(0);
    _channels[channel]->diffuseMessage(msg);
}

void Server::mode(std::string str, int socket)
{
    str = str.substr(5);
    str.erase(str.find_last_not_of('\n'));
    std::vector<std::string> commands = splitCommands(str, ' ');
    if(!(commands.size() <= 3 && commands.size() >= 2))
        return ;
    std::string channel = commands[0];
    std::string mode = commands[1];
    std::string nickname = commands[2];
    if (_channels.find(channel) != _channels.end())
    {
        if (!_channels[channel]->isOperator(socket))
        {
            std::string msg = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
        if ((_modes.find(std::string(1, mode[1])) != _modes.end()) && (mode.size() == 2 || (mode[0] == '-' && mode[1] == 'l')))
        {
            mode[0] == '+' ? (this->*_modes[std::string(1, mode[1])])(1, channel, nickname, socket)
                           : (this->*_modes[std::string(1, mode[1])])(0, channel, nickname, socket);
        }
        else
        {
            std::string msg = ERR_UNKNOWNMODE(_clients[socket]->getNickname(), mode);
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}

void Server::kick(std::string str, int socket)
{
    std::vector <std::string> commands = splitCommands(str, '\n');
    std::vector <std::string>::iterator it = commands.begin();
    for (; it != commands.end(); it++)
    {
        std::string word = *it;
        std::istringstream iss(word);
        iss >> word;
        iss >> word;
        if (_channels.find(word) != _channels.end())
        {
            if(_channels[word]->isOperator(socket))
            {
                std::string channel = word;
                iss >> word;
                std::string target = word;
                int targetSocket = getClientSocket(target);
                if (_channels[channel]->isInChannel(targetSocket))
                {
                    iss >> word;
                    word = word.substr(1);   
                    std::string msg = word.empty()  ? KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "Kicked by operator", channel, target)
                                                    : KICK_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), word, channel, target);
                    _clients[targetSocket]->removeChannel(_channels[channel]);
                    _channels[channel]->removeOperator(targetSocket);
                    _channels[channel]->removeSocket(targetSocket);     
                    _channels[channel]->diffuseMessage(msg);

                }
                else
                {
                    std::string msg = ERR_NOTONCHANNEL(_clients[targetSocket]->getNickname(), word);
                    send(socket, msg.c_str(), msg.size(), 0);
                }
            }
            else
            {
                std::string msg = _channels[word]->isInChannel(socket) 
                                  ? ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), word.substr(1))
                                  : ERR_NOTONCHANNEL(_clients[socket]->getNickname(), word);
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), word);
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
}

Channel *Server::getChannel(std::string name)
{
    if (_channels.find(name) != _channels.end())
        return _channels[name];
    return NULL;
}

void Server::list(std::string str, int socket)
{
    std::istringstream iss(str.substr(5));
    std::string word;
    iss >> word;

    std::string start = RPL_LISTSTART(_clients[socket]->getNickname());
    std::string end = RPL_LISTEND(_clients[socket]->getNickname());
    send(socket, start.c_str(), start.size(), 0);
    if (!word.empty())
    {
        std::vector<std::string> channels = splitCommands(word, ',');
        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            *it = '#' + *it;
            if (_channels.find(*it) != _channels.end())
            {
                std::ostringstream oss;
                oss << _channels[*it]->getSockets().size();
                std::string socketsSizeStr = oss.str();
                std::string msg = RPL_LIST(_clients[socket]->getNickname(), (*it).substr(1), socketsSizeStr, _channels[*it]->getTopic());
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
    }
    else
    {
        for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            std::ostringstream oss;
            oss << it->second->getSockets().size();
            std::string socketsSizeStr = oss.str();
            std::string msg = RPL_LIST(_clients[socket]->getNickname(), it->first.substr(1), socketsSizeStr, it->second->getTopic());
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    send(socket, end.c_str(), end.size(), 0);
}

void Server::invite(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    std::string target = word;
    iss >> word;
    std::string channel = word;
    
    if (_channels.find(channel) != _channels.end())
    {
        if (_channels[channel]->isOperator(socket))
        {
            if (getClientSocket(target) && !_channels[channel]->isInChannel(getClientSocket(target)))
            {
                _channels[channel]->setInvited(getClientSocket(target));
                std::string msg = RPL_INVITING(_clients[socket]->getNickname(), target, channel);
                std::string msg2 = INVITE_CLIENT(_clients[socket]->getNickname(), _clients[socket]->getUsername(), "INVITE", target, channel);
                send(socket, msg.c_str(), msg.size(), 0);
                send(getClientSocket(target), msg2.c_str(), msg2.size(), 0);
            }
            else if(_channels[channel]->isInChannel(getClientSocket(target)))
            {
                std::string msg = ERR_USERONCHANNEL(target, channel);
                send(socket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                std::string msg = ERR_NOSUCHNICK(_clients[socket]->getNickname(), target);
                send(socket, msg.c_str(), msg.size(), 0);
            }
        }
        else
        {
            std::string msg = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg.c_str(), msg.size(), 0);
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), channel);
        send(socket, msg.c_str(), msg.size(), 0);
    }
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
    str = str.substr(5);
    str.erase(str.find_last_not_of('\n'));
    std::vector<std::string> commands = splitCommands(str, ' ');
    std::string channel = commands[0];
    std::string password = commands[1];
    if (channel[0] == '#')
    {
		if (_channels.find(channel) != _channels.end())
        {
            if (_channels[channel]->getInviteOnly() && !_channels[channel]->isInvited(socket))
            {
                std::string msg = ERR_INVITEONLYCHAN(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_channels[channel]->getLimit() && _channels[channel]->getSockets().size() >= _channels[channel]->getLimit())
            {
                std::string msg = ERR_CHANNELISFULL(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (!_channels[channel]->getPassword().empty() && _channels[channel]->getPassword() != password)
            {
                std::string msg = ERR_BADCHANNELKEY(_clients[socket]->getNickname(), channel);
                send(socket, msg.c_str(), msg.size(), 0);
                return ;
            }
            if (_channels[channel]->isInvited(socket))
                _channels[channel]->removeInvited(socket);
           _channels[channel]->addSocket(socket);
           _clients[socket]->addChannel(_channels[channel]);
           std::string msg = RPL_JOIN(_clients[socket]->getNickname(), channel.substr(1));
           send(socket, msg.c_str(), msg.size(), 0);
           if (_channels[channel]->getTopic().empty())
           {
                std::string msg2 = RPL_NOTOPIC(_clients[socket]->getNickname(), channel.substr(1));
                send(socket, msg2.c_str(), msg2.size(), 0);
           }
           else
           {
                std::ostringstream oss;
                oss << std::time(0);
                std::string time = oss.str();
                std::string msg2 = RPL_TOPIC(_clients[socket]->getNickname(), channel.substr(1), _channels[channel]->getTopic());\
                send(socket, msg2.c_str(), msg2.size(), 0);
                std::string msg3 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), channel.substr(1), _channels[channel]->getTopicSetter(), time);
                send(socket, msg3.c_str(), msg3.size(), 0);
           }
        }
        else
        {
            _channels[channel] = new Channel(channel);
            _channels[channel]->addSocket(socket);
            _channels[channel]->setOperator(socket);
            _clients[socket]->addChannel(_channels[channel]);
			std::string msg = RPL_JOIN(_clients[socket]->getNickname(), channel.substr(1));            
    		send(socket, msg.c_str(), msg.size(), 0);
            std::string msg2 = RPL_NOTOPIC(_clients[socket]->getNickname(), channel.substr(1));
            send(socket, msg2.c_str(), msg2.size(), 0);
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
        if ((_channels.find(word) == _channels.end()) || (!_channels[word]->isInChannel(socket)))
	    {
            std::string msg = ERR_CANNOTSENDTOCHAN(_clients[socket]->getNickname(), word);
            send(socket, msg.c_str(), msg.size(), 0);
            return ;
        }
    	std::string msg = RPL_PRIVMSG_CHANNEL((CYAN + _clients[socket]->getNickname() + RESET), word, toSend);
		_channels[word]->sendChanMsg(socket, msg);
        return ;
	}
    else
    {
        for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            if (it->second->getNickname() == word)
            {
    	        std::string msg = RPL_PRIVMSG_CHANNEL((RED +_clients[socket]->getNickname() + RESET), ("#" + word), toSend);
                std::cout << msg << std::endl;
                send(it->second->getSocket(), msg.c_str(), msg.size(), 0);
                return ;
            }
        }
	}
    std::string msg = _clients[socket]->getNickname() + " :No user with this name\r\n";
    send(socket, msg.c_str(), msg.size(), 0);
}

int Server::getServerSocket() {return this->_serverSocket;}

void Server::topic(std::string str, int socket)
{
    std::istringstream iss(str);
    std::ostringstream oss;
    oss << std::time(0);
    std::string word;
    iss >> word;
    iss >> word;
    std::string currentTime = oss.str();

    if (_channels.find(word) != _channels.end())
    {
        std::string toSend;
        size_t pos = str.find(':');
        if (pos != std::string::npos)
        {
            if (_channels[word]->getRestrictedTopic() && !_channels[word]->isOperator(socket))
            {
                toSend = ERR_CHANOPRIVSNEED(_clients[socket]->getNickname(), word.substr(1));
                send(socket, toSend.c_str(), toSend.size(), 0);
                return ;
            }
            _channels[word]->setTopic(str.substr(pos + 1));
            _channels[word]->setTopicSetter(_clients[socket]);
            std::string msg = RPL_TOPIC(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopic());
            std::string msg2 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopicSetter(), currentTime);
            std::vector<int> sockets = _channels[word]->getSockets();
            for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); it++)
            {
                send(*it, msg.c_str(), msg.size(), 0);
                send(*it, msg2.c_str(), msg2.size(), 0);
            }
        }
        else
        {

            if (_channels[word]->getTopic().empty())
            {
                std::string msg = RPL_NOTOPIC(_clients[socket]->getNickname(), word.substr(1));
                send(socket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                std::string msg = RPL_TOPIC(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopic());
                std::string msg2 = RPL_TOPICWHOTIME(_clients[socket]->getNickname(), word.substr(1), _channels[word]->getTopicSetter(), currentTime);
                send(socket, msg.c_str(), msg.size(), 0);
                send(socket, msg2.c_str(), msg2.size(), 0);
            }
        }
    }
    else
    {
        std::string msg = ERR_NOSUCHCHANNEL(_clients[socket]->getNickname(), word);
        send(socket, msg.c_str(), msg.size(), 0);
    }
}

void    Server::whois(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
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
        std::string msg = ":localhost 001 " + sub + " :Nickname changed to " + sub + "\n";
        send(socket, msg.c_str(), msg.size(), 0);
    }
}


void Server::pass(std::string str, int socket)
{
    std::string pass = str.substr(5, this->getPassLength());
    if (pass == this->getPassword())
    {
        std::string msg = "Correct Password !\n";
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(1);
    }
    else
    {
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        _clients[socket]->setIsConnect(0);
    }
}

void Server::user(std::string str, int socket)
{
    std::istringstream iss(str);
    std::string word;
    iss >> word;
    iss >> word;
    if (word.empty())
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
    std::string word = str.substr(6);
    _clients[socket]->diffuseMessage("Quit: " + word);
    _clients[socket]->removeAllChannels();
    close(socket);
    delete _clients[socket];
    _clients.erase(socket);
}

void Server::capls(std::string str, int socket)
{
   std::vector<std::string> commands = splitCommands(str, '\n');
   std::vector<std::string>::iterator it = commands.begin();

   if (it != commands.end())
       ++it;

   for(; it != commands.end(); it++)
   {
        std::string word = *it;
        (this->*_funcTab[word.substr(0,4)])(word,socket);
   }
   if (_clients[socket]->getIsConnect() == 0)
    {
        std::string msg = ERR_PASSWDMISMATCH(_clients[socket]->getNickname());
        send(socket, msg.c_str(), msg.size(), 0);
        close(socket);
        _clients.erase(socket);
        return ;
    }
}

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
                    std::cout << "\033[35m"<< "Receive : |" << buffer << "|\033[0m" << std::endl;
                    std::string neww(buffer);
                    if (this->_funcTab.find(firstWord(neww)) != this->_funcTab.end())
                    {
                        std::string neww(buffer);
                        (this->*_funcTab[firstWord(neww)])(neww, _events[i].data.fd);
                    }
                    memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}