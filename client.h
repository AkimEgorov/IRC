#ifndef _IRCCLIENT_H
#define _IRCCLIENT_H

#include <string>
#include <vector>
#include "socket.h"

class IRCClient;

struct IRCCommandPrefix
{
    void Parse(std::string data)
    {
        if (data == "")
            return;

        prefix = data.substr(1, data.find(" ") - 1);
        std::vector<std::string> tokens;

        if (prefix.find("@") != std::string::npos)
        {
            size_t start = 0, end = 0;
            std::vector<std::string> tokens;

            while ((end = prefix.find('@', start)) != std::string::npos)
            {
                tokens.push_back(prefix.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(prefix.substr(start));

            nick = tokens.at(0);
            host = tokens.at(1);
            
        }
        if (nick != "" && nick.find("!") != std::string::npos)
        {
            std::vector<std::string> tokens;

            size_t start = 0, end = 0;
            while ((end = nick.find('!', start)) != std::string::npos)
            {
                tokens.push_back(nick.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(nick.substr(start));

            nick = tokens.at(0);
            user = tokens.at(1);
        }
    };

    std::string prefix;
    std::string nick;
    std::string user;
    std::string host;
};

struct IRCMessage
{
    IRCMessage();
    IRCMessage(std::string cmd, IRCCommandPrefix p, std::vector<std::string> params) :
        command(cmd), prefix(p), parameters(params) {};

    std::string command;
    IRCCommandPrefix prefix;
    std::vector<std::string> parameters;
};

class IRCClient
{
public:

    bool InitSocket();
    bool Connect(char* /*host*/, int /*port*/);
    void Disconnect();
    bool Connected() { return _socket.Connected(); };

    bool SendIRC(std::string /*data*/);
    bool Login(std::string /*nick*/, std::string /*user*/, std::string /*password*/ = std::string());
    void Parse(std::string /*data*/);
    void ReceiveData();

    void HPrivMsg(IRCMessage /*message*/);
    void HChannelJoin(IRCMessage /*message*/);
    void HNickChange(IRCMessage /*message*/);
    void HUserQuit(IRCMessage /*message*/);
    void HChannelUList(IRCMessage /*message*/);
    void HNicknameCurrent(IRCMessage /*message*/);
    void HServerMessage(IRCMessage /*message*/);



private:

    IRCSocket _socket;

    std::string _nick;
    std::string _user;

};
#endif