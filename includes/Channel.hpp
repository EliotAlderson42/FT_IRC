#pragma once
#include "Lib.hpp"

class Channel {
    public:
    Channel();
    Channel(std::string name);
    ~Channel();
    void    sendChanMsg(int sock, std::string msg);
    void    diffuseMessage(std::string msg);
    void    addSocket(int socket);
    std::string getPassword() const;
    void    setOperator(int socket);
    void    removeOperator(int socket);
    std::string   getName() const;
    std::string getTopic() const;
    void    setTopic(std::string topic);
    void    setTopicSetter(Client *client);
    void    setPassword(std::string password);
    std::string getTopicSetter() const;
    std::vector<int> getSockets() const;
    bool   isOperator(int socket) const;
    void   removeSocket(int socket);
    bool   isInChannel(int socket) const;
    void   setInvited(int socket);
    void   removeInvited(int socket);
    bool   isInvited(int socket) const;
    void   setInviteOnly(bool value);
    void   setRestrictedTopic(bool value);
    bool   getRestrictedTopic() const;
    bool   getInviteOnly() const;
    void   setLimit(int limit);
    int    getLimit() const;

    private:
    std::vector<int> _invited;
    std::vector<int> _operators;
    int _nbUser = 0;
    std::string _name;
    std::string _password;
    std::vector<int> _sockets;
    std::string _topic;
    std::string _topicSetter;
    bool _inviteOnly;
    bool _restrictedTopic;
    int _limit = 0;
};