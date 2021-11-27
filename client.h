#ifndef _IRCCLIENT_H
#define _IRCCLIENT_H

#include <string>
#include <vector>
#include "socket.h"

class IRCClient;

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


private:

    IRCSocket _socket;

    std::string _nick;
    std::string _user;

};
#endif