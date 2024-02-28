#pragma once
#include "Lib.hpp"

class Channel {
    public:

    //Constructors and destructors
    Channel(std::string name);
    Channel();
    ~Channel();

    //Getters
    std::vector<int>    getSockets()            const;
    std::string         getTopic()              const;
    std::string         getTopicSetter()        const;
    std::string         getName()               const;
    std::string         getPassword()           const;
    size_t              getLimit()              const;
    bool                getRestrictedTopic()    const;
    bool                getInviteOnly()         const;

    //Setters
    void    setPassword(std::string password);
    void    setRestrictedTopic(bool value);
    void    setTopicSetter(Client *client);
    void    setTopic(std::string topic);
    void    setInviteOnly(bool value);
    void    setOperator(int socket);
    void    setInvited(int socket);
    void    setLimit(int limit);

    //Methods
    void   sendChanMsg(int sock, std::string msg);
    void   diffuseMessage(std::string msg);
    bool   isInChannel(int socket) const;
    bool   isOperator(int socket) const;
    bool   isInvited(int socket) const;
    void   removeOperator(int socket);
    void   removeInvited(int socket);
    void   removeSocket(int socket);
    void   addSocket(int socket);


    private:
    std::vector<int>    _operators;
    std::vector<int>    _invited;
    std::vector<int>    _sockets;
    std::string         _topicSetter;
    std::string         _password;
    std::string         _topic;
    std::string         _name;
    size_t              _limit;
    bool                _inviteOnly;
    bool                _restrictedTopic;
};