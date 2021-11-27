//
// Created by itmoster on 27.11.2021.
//

#ifndef IRC_HANDLER_H
#define IRC_HANDLER_H

#include "handler.h"

#define N_IRC_CMD 24;

struct cmdHandler {
    std::string command;
    void (IRCClient::*handler)(IRCMessage /*message*/);
};

IRCCommandHandler ircCommandTable[N_IRC_CMD];

inline int GetCommandHandler(std::string command)
{
    for (int i = 0; i < N_IRC_CMD; ++i)
    {
        if (ircCommandTable[i].command == command)
            return i;
    }

    return NUM_IRC_CMDS;
}


#endif //IRC_HANDLER_H
