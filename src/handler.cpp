#include "handler.h"

cmdHandler ircCommandTable[N_IRC_CMD] =
        {
                { "PRIVMSG",            &IRCClient::HPrivMsg                   },
                { "INVITE",             &IRCClient::HInviteMessage             },
                { "KICK",               &IRCClient::HKickMessage               },
                { "JOIN",               &IRCClient::HChannelJoinPart           },
                { "PART",               &IRCClient::HChannelJoinPart           },
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
                { "341",                &IRCClient::HServerInvtMessage         },
                { "401",                &IRCClient::HServerMessage             },
        };

void IRCClient::HPrivMsg(IRCMessage message)
{
    std::string to = message.parameters.at(0);
    std::string text = message.parameters.at(message.parameters.size() - 1);
    //init_pair(5, COLOR_CYAN, COLOR_BLACK);
    //attron(COLOR_PAIR(5));

    if (to[0] == '#')
        std::cout << "\x1b[34;2m" << "<" << message.prefix.nick << ">" << "\x1b[35;2m" << " @ " + to + ": " << "\x1b[0m" << text << std::endl;
    else
        std::cout << "\x1b[34;2m" << "<" << message.prefix.nick << ">" << ": " << "\x1b[0m" << text << std::endl;
}


void IRCClient::HChannelJoinPart(IRCMessage message)
{
    std::string channel = message.parameters.at(0);
    std::string action = message.command == "JOIN" ? "joins" : "leaves";
    std::cout << "\x1b[34;2m" << "<" << message.prefix.nick << ">" << " " << "\x1b[0m" << action << " " << "\x1b[35;2m" << channel << "\x1b[0m" << std::endl;
}

void IRCClient::HNickChange(IRCMessage message)
{
    std::string newNick = message.parameters.at(0);
    std::cout << "\x1b[34;2m" << "<" << message.prefix.nick << ">" << "\x1b[0m" << " changed his nick to " << "\x1b[34;2m" << "<" << newNick << ">" << "\x1b[0m" << std::endl;
}

void IRCClient::HUserQuit(IRCMessage message)
{
    std::string text = message.parameters.at(0);
    std::cout << "\x1b[34;2m" << "<" << message.prefix.nick << ">" << "\x1b[31m" << " quits (" << text << ")" << "\x1b[0m" << std::endl;
}

void IRCClient::HChannelUList(IRCMessage message)
{
    std::string channel = message.parameters.at(2);
    std::string nicks = message.parameters.at(3);
    std::cout << "People on " << "\x1b[35;2m" << channel << ":" << std::endl << "\x1b[34;2m" << "<" << nicks << ">" << "\x1b[0m" << std::endl;
}

void IRCClient::HNicknameCurrent(IRCMessage message)
{
    std::cout << message.parameters.at(1) << " " << message.parameters.at(2) << std::endl;
}

void IRCClient::HServerMessage(IRCMessage message)
{
    if (message.parameters.empty())
        return;

    std::vector<std::string>::const_iterator itr = message.parameters.begin();
    ++itr; // skip the first parameter (our nick)
    //start_color();
    //init_pair(5, COLOR_CYAN, COLOR_BLACK);
    //std::string a;
    for (; itr != message.parameters.end(); ++itr)
    {
       // attron(COLOR_PAIR(5));
        //std::string a;
        //a = *itr + " ";
        //printw(a.c_str());
        std::cout << *itr << " ";
        //attroff(COLOR_PAIR(5));
    }
    std::cout << std::endl;
    
}

void IRCClient::HInviteMessage(IRCMessage message)
{
    std::string nick = message.prefix.nick;
    std::string channel = message.parameters.at(1);
    std::cout << "\x1b[34;2m" << "<" << nick << ">" << "\x1b[0m" << " invites you to join " << "\x1b[35;2m" << channel << "\x1b[0m" << std::endl;
}

void IRCClient::HKickMessage(IRCMessage message)
{
    std::string nick1 = message.prefix.nick;
    std::string channel = message.parameters.at(0);
    std::string nick2 = message.parameters.at(1);
    std::cout << "\x1b[34;2m" << "<" << nick1 << ">" << "\x1b[0m" << " removed " << "\x1b[34;2m" << "<" << nick2 << ">" << "\x1b[0m" << " from channel " << "\x1b[35;2m" << channel << "\x1b[0m" << std::endl;
}

void IRCClient::HServerInvtMessage(IRCMessage message)
{
    std::string nick = message.parameters.at(1);
    //std::string channel = message.parameters.at(2);
    std::cout << "You invited " << "\x1b[34;2m" << "<" << nick << ">" << "\x1b[0m" << " into the channel" << std::endl;
}