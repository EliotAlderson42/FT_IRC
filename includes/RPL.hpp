#ifndef RPL_HPP
# define  RPL_HPP

# include "Lib.hpp"

# define user_id(nickname, username, cmd) (":" + nickname + "!~" + username + "@localhost " + cmd + " ") //

            /* = = =    TO CONNECT    = = = */
// 001
# define RPL_WELCOME(nickname) (":localhost 001 " +  nickname + " :Welcome to the Internet Relay Network " + nickname + "\r\n") //

            /* = = =    MODE    = = = */
# define SET_channel_MODE(nickname, username, cmd, channel, mode) (user_id(nickname, username, cmd) + "#" + channel + " +" + mode + "\r\n") //

# define UNSET_channel_MODE(nickname, username, cmd, channel, mode) (user_id(nickname, username, cmd) + "#" +channel + " -" + mode + "\r\n") //

# define SET_NEWOPER(nickname, username, cmd, channel, mode, concerned_client_nickname) ((user_id(nickname, username, cmd) + "#" + channel + " +" + mode  + " " + concerned_client_nickname + "\r\n")) //

# define UNSET_OPER(nickname, username, cmd, channel, mode, concerned_client_nickname) ((user_id(nickname, username, cmd) + "#" + channel + " -" + mode  + " " + concerned_client_nickname + "\r\n")) //
// 324
# define RPL_CHANNELMODEIS(nickname, channel, channel_mods) (":localhost 324 " + nickname + " #" + channel + " +" + channel_mods + "\r\n") //
// 481
# define ERR_NOPRIVILEGES(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You cannot destitute the channel funder\r\n") //

# define ERR_INVALIDLIMIT(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " : The limit you tried to set in unvalid\r\n") //
// 482
# define ERR_CHANOPRIVSNEED(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You're not channel operator\r\n") //
// 501
# define ERR_UMODEUNKNOWNFLAG(nickname) (":localhost 501 " + nickname + " :Unknown MODE flag\r\n") //
// 472
# define ERR_UNKNOWNMODE(nickname, mode) (":localhost 472 " + nickname + " " + mode + " :is unknown mode char to me for " + nickname + "\r\n") //
            /* = = =    PASS    = = = */
// 464
# define ERR_PASSWDMISMATCH(nickname) (":localhost 464 " + nickname + " :Password incorrect.\r\n")//

            /* = = =    WHO     = = = */
// 311
# define RPL_WHOISUSER(nickname, host_name, username) (":localhost 311 " + nickname + " " + host_name + " ~" + username + " localhost * :" + username + "\r\n") //working
// 318
# define RPL_ENDOFWHOIS(nickname) (":localhost 318: " + nickname + " :End of /WHOIS\r\n")//working
            /* = = =    NICK    = = = */
// 401
# define ERR_NOSUCHNICK(nickname, wrong_nick) (":localhost 401: " + nickname + " There is no such nick : " + wrong_nick + "\r\n") //
// 433
# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 * " + nickname + " :Nickname is already in use\r\n") //
// 431
# define ERR_NONICKNAMEGIVEN(nickname) (":localhost 431 * " + nickname + " :No nickname given\r\n") //
            /* = = =    GENERIC ERR     = = = */
// 461
# define ERR_NEEDMOREPARAMS(nickname, cmd) (":localhost 461 " + nickname + " " + cmd + " :Not enough parameters\r\n") //
// 462
# define ERR_ALREADYREGISTRED(nickname) (":localhost 462 " + nickname + " :You may not reregister\r\n") //
            /* = = =    PING / PONG     = = = */
# define RPL_PONG ":localhost PONG localhost :localhost\r\n" //used

            /* = = =    JOIN     = = = */
# define RPL_JOIN(nickname, channel) (':' + nickname + " JOIN #" + channel + "\r\n") //used

# define PART_channel(nickname, username, cmd, channel) (user_id(nickname, username, cmd) + channel + "\r\n") //

// 353
# define RPL_NAMREPLY(nickname, channel, list_client) (":localhost 353 " + nickname + " = " + channel + " :" + list_client +"\r\n") //
// 366
# define RPL_ENDOFNAMES(nickname, channel) (":localhost 366 " + nickname + " " + channel + " :End of /NAMES list.\r\n")//
// 471
# define ERR_CHANNELISFULL(nickname, channel) (":localhost 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n") //
// 475
# define ERR_BADCHANNELKEY(nickname, channel) (":localhost 475 " + nickname + " " + channel + " :\r\n") //
// 403
# define ERR_NOSUCHCHANNEL(nickname, channel) (":localhost 403 " + nickname + " " + channel + " :No such channel\r\n") //
// 404
# define ERR_CANNOTSENDTOCHAN(nickname, channel) (":localhost 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")//
// 441
# define ERR_USERNOTINCHANNEL(nickname, concerned_client_nickname, channel) ((":localhost 441 " + nickname + " " + concerned_client_nickname + " " + channel + " :They aren't on that channel\r\n")) //
// 442
# define ERR_NOTONCHANNEL(nickname, channel) (":localhost 442 " + nickname + " #" + channel + " :You're not on that channel\r\n") //
// 443
# define ERR_USERONCHANNEL(nickname, channel) (":localhost 443 " + channel + " " + nickname + " :is already on channel\r\n") //

# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 * " + nickname + " :Erroneous nickname\r\n")

            /* = = =    PRV MESSAGES     = = = */
# define RPL_PRIVMSG_CHANNEL(nickname, channel, msg) (":" + nickname + " PRIVMSG " + channel + " " + msg + "\r\n") //used

# define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + "eliotalderson" + " " + command + " ")
# define RPL_PRIVMSG_CLIENT(nickname, username, dest, msg) (CLIENT_ID(nickname, username, "PRIVMSG") + dest + " :" + msg + "\r\n") //used

# define ERR_NOTEXTTOSEND(nickname) (nickname + " :No text to send\r\n") //

            /* = = =    INVITE     = = = */
// 341
# define RPL_INVITING(nickname, target, channel) (":localhost 341 " + nickname + " " + target + " :" + channel + "\r\n") //

# define INVITE_CLIENT(nickname, username, cmd, concerned_client_nickname, channel) (user_id(nickname, username, cmd) + concerned_client_nickname + " :" + channel + "\r\n") //

# define NOTICE_CLIENT_INVITE(nickname, channel) (nickname + " invites you to " + channel + "\r\n") //
// 473
# define ERR_INVITEONLYCHAN(nickname, channel) (":localhost 473 " +  nickname + " " + channel + " :Cannot join channel (+i)\r\n") //

            /* = = =    KICK     = = = */
#define KICK_CLIENT(nickname, username, cmd, channel, concerned_client_nickname) ((user_id(nickname, username, cmd)) + channel + " " + concerned_client_nickname + " :\r\n") //

            /* = = =    TOPIC     = = = */
// 331
#define RPL_NOTOPIC(nickname, channel) (":localhost 331 " + nickname + " #" + channel + " :No topic is set\r\n") //
// 332
#define RPL_TOPIC(nickname, channel, topic) (":localhost 332 " + nickname + " #" + channel + " : " + topic + "\r\n") //
// 333
#define RPL_TOPICWHOTIME(nickname, channel, nickname_who, time) (":localhost 333 " + nickname + " #" + channel + " " + nickname_who + " " + time + "\r\n") //
            /* = = =    LIST     = = = */

// 321
#define RPL_LISTSTART(nickname) (":localhost 321 " + nickname + " :Channel :Users Name\r\n") //
// 322
#define RPL_LIST(nickname, channel, nb_users, topic) (":localhost 322 " + nickname + " #" + channel + " " + nb_users + " :" + topic + "\r\n") //
// 323
#define RPL_LISTEND(nickname) (":localhost 323 " + nickname + " :End of /LIST\r\n") //

            /* = = =    QUIT     = = = */
// #define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + channel + " " + command + " ")
// #define  NICK_RPL(nickname, username, new_nickname) (CLIENT_ID(nickname, username, "NICK") + ":" + new_nickname + "\r\n")
#endif