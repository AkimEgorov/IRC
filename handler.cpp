//
// Created by itmoster on 27.11.2021.
//

#include "IRCHandler.h"

IRCCommandHandler ircCommandTable[NUM_IRC_CMDS] =
        {
                { "PRIVMSG",            &IRCClient::HPrivMsg                   },
                { "JOIN",               &IRCClient::HChannelJoin               },
                { "NICK",               &IRCClient::HNickChange                },
                { "QUIT",               &IRCClient::HUserQuit                  },
                { "353",                &IRCClient::HChannelUList              },
                { "433",                &IRCClient::HNicknameCurrent           },
                { "001",                &IRCClient::HServerMessage             },
                { "002",                &IRCClient::HServerMessage             },
                { "003",                &IRCClient::HServerMessage             },
                { "004",                &IRCClient::HServerMessage             },
                { "005",                &IRCClient::HServerMessage             },
                { "250",                &IRCClient::HServerMessage             },
                { "251",                &IRCClient::HServerMessage             },
                { "252",                &IRCClient::HServerMessage             },
                { "253",                &IRCClient::HServerMessage             },
                { "254",                &IRCClient::HServerMessage             },
                { "255",                &IRCClient::HServerMessage             },
                { "265",                &IRCClient::HServerMessage             },
                { "266",                &IRCClient::HServerMessage             },
                { "366",                &IRCClient::HServerMessage             },
                { "372",                &IRCClient::HServerMessage             },
                { "375",                &IRCClient::HServerMessage             },
                { "376",                &IRCClient::HServerMessage             },
                { "439",                &IRCClient::HServerMessage             },
        };

void IRCClient::HandlePrivMsg(IRCMessage message)
{
    std::string to = message.parameters.at(0);
    std::string text = message.parameters.at(message.parameters.size() - 1);

    if (to[0] == '#')
        std::cout << "From " + message.prefix.nick << " @ " + to + ": " << text << std::endl;
    else
        std::cout << "From " + message.prefix.nick << ": " << text << std::endl;
}


void IRCClient::HChannelJoin(IRCMessage message)
{
    std::string channel = message.parameters.at(0);
    std::string action = message.command == "JOIN" ? "joins" : "leaves";
    std::cout << message.prefix.nick << " " << action << " " << channel << std::endl;
}

void IRCClient::HNickChange(IRCMessage message)
{
    std::string newNick = message.parameters.at(0);
    std::cout << message.prefix.nick << " changed his nick to " << newNick << std::endl;
}

void IRCClient::HUserQuit(IRCMessage message)
{
    std::string text = message.parameters.at(0);
    std::cout << message.prefix.nick << " quits (" << text << ")" << std::endl;
}

void IRCClient::HChannelUList(IRCMessage message)
{
    std::string channel = message.parameters.at(2);
    std::string nicks = message.parameters.at(3);
    std::cout << "People on " << channel << ":" << std::endl << nicks << std::endl;
}

void IRCClient::HNicknameCurrent(IRCMessage message)
{
    std::cout << message.parameters.at(1) << " " << message.parameters.at(2) << std::endl;
}

void IRCClient::HServerMessage(IRCMessage message)
{
    if( message.parameters.empty() )
        return;

    for (int i = message.parameters.begin(); i != message.parameters.end(); i++) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}
