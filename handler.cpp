#include "handler.hpp"

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
        };

std::string IRCClient::HPrivMsg(IRCMessage message)
{
    std::string to = message.parameters.at(0);
    std::string text = message.parameters.at(message.parameters.size() - 1);

    if (to[0] == '#'){
        std::string temp = "From " + message.prefix.nick +" @ " + to + ": "+ text;
        return temp;
    }
    else{
         std::string temp = "From " + message.prefix.nick + ": " + text;
         return temp;
    }
}


std::string IRCClient::HChannelJoinPart(IRCMessage message)
{
    std::string channel = message.parameters.at(0);
    std::string action = message.command == "JOIN" ? "joins" : "leaves";
    std::string temp = message.prefix.nick + " " + action + " " + channel ;
    return temp;
}

std::string IRCClient::HNickChange(IRCMessage message)
{
    std::string newNick = message.parameters.at(0);
    std::string temp = message.prefix.nick + " changed his nick to " + newNick ;
    return temp;
}

std::string IRCClient::HUserQuit(IRCMessage message)
{
    std::string text = message.parameters.at(0);
    std::string temp = message.prefix.nick + " quits (" + text + ")" ;
    return temp;
}

std::string IRCClient::HChannelUList(IRCMessage message)
{
    std::string channel = message.parameters.at(2);
    std::string nicks = message.parameters.at(3);
    std::string temp ="People on " + channel + ":" + nicks ;
    return temp;
}

std::string IRCClient::HNicknameCurrent(IRCMessage message)
{
    std::string temp = message.parameters.at(1) + " " + message.parameters.at(2);
    return temp;
}

std::string IRCClient::HServerMessage(IRCMessage message)
{
    if (message.parameters.empty())
        return "";
    std::string temp="";
    std::vector<std::string>::const_iterator itr = message.parameters.begin();
    ++itr; // skip the first parameter (our nick)
    for (; itr != message.parameters.end(); ++itr)
        temp+= *itr+ " ";
    return temp;
    
}

std::string IRCClient::HInviteMessage(IRCMessage message)
{
    std::string nick = message.prefix.nick;
    std::string channel = message.parameters.at(1);
    std::string temp = nick + " invites you to join " + channel;
    return temp;
}

std::string IRCClient::HKickMessage(IRCMessage message)
{
    std::string nick1 = message.prefix.nick;
    std::string channel = message.parameters.at(0);
    std::string nick2 = message.parameters.at(1);
    std::string temp =  nick1 + " removed " + nick2 + " from channel " + channel ;
    return temp;
}
