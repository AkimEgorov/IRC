#ifndef HANDLER_H
#define HANDLER_H
#include "client.h"

#define N_IRC_CMD 29

struct cmdHandler {
    std::string command;
    void (IRCClient::*handler)(IRCMessage /*message*/);
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


#endif //HANDLER_H
