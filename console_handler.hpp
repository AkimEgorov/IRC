#pragma once


#include <cstdlib>
#include <map>
#include <algorithm>
#include "thread.hpp"
#include "client.hpp"


class ConsoleCommandHandler
{
public:
    bool AddCommand(std::string name, int argCount, void (*handler)(std::string /*params*/, IRCClient* /*client*/));

    void ParseCommand(std::string command, IRCClient* client);

private:
    struct CommandEntry
    {
        int argCount;
        void (*handler)(std::string /*arguments*/, IRCClient* /*client*/);
    };

    std::map<std::string, CommandEntry> _commands;
};


void msgCommand(std::string arguments, IRCClient* client);

void joinCommand(std::string channel, IRCClient* client);

void partCommand(std::string channel, IRCClient* client);

void invtCommand(std::string arguments, IRCClient* client);

void kickCommand(std::string arguments, IRCClient* client);

void helpCommand(std::string channel, IRCClient* client);
