#include <iostream>
#include <algorithm>
#include "socket.h"
#include "client.h"

bool IRCClient::InitSocket()
{
    return _socket.Init();
}

bool IRCClient::Connect(char* host, int port)
{
    return _socket.Connect(host, port);
}

void IRCClient::Disconnect()
{
    _socket.Disconnect();
}

bool IRCClient::SendIRC(std::string data)
{
    data.append("\n");
    return _socket.SendData(data.c_str());
}

bool IRCClient::Login(std::string nick, std::string user, std::string password)
{
    _nick = nick;
    _user = user;

    if (SendIRC("HELLO"))
    {
        if (!password.empty() && !SendIRC("PASS " + password))
            return false;
        if (SendIRC("NICK " + nick))
            if (SendIRC("USER " + user + " 8 * :Cpp IRC Client"))
                return true;
    }

    return false;
}