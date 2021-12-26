#pragma once


#include "client.hpp"

#define N_IRC_CMD 27

struct cmdHandler {
    std::string command;
    std::string (IRCClient::*handler)(IRCMessage /*message*/);
};

extern cmdHandler ircCommandTable[N_IRC_CMD];

inline int GetCommandHandler(std::string command)
{
    for (int i = 0; i < N_IRC_CMD; ++i)
    {
        if (ircCommandTable[i].command == command)
            return i;
    }

    return N_IRC_CMD;
}

