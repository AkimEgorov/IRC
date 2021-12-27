#include "console_handler.h"

bool ConsoleCommandHandler::AddCommand(std::string name, int argCount, void (*handler)(std::string /*params*/, IRCClient* /*client*/))
{
    CommandEntry entry;
    entry.argCount = argCount;
    entry.handler = handler;
    std::transform(name.begin(), name.end(), name.begin(), towlower);
    _commands.insert(std::pair<std::string, CommandEntry>(name, entry));
    return true;
}

void ConsoleCommandHandler::ParseCommand(std::string command, IRCClient* client)
{
    if (_commands.empty())
    {
        std::cout <<"\x1b[31;1m"<< "No commands available."<<"\x1b[0m" << std::endl;
        return;
    }

    if (command[0] == '/')
        command = command.substr(1); // Remove the slash

    std::string name = command.substr(0, command.find(" "));
    std::string args = command.substr(command.find(" ") + 1);
    int argCount = std::count(args.begin(), args.end(), ' ');

    std::transform(name.begin(), name.end(), name.begin(), towlower);

    std::map<std::string, CommandEntry>::const_iterator itr = _commands.find(name);

    if (itr == _commands.end())
    {
        std::cout <<"\x1b[31;1m"<< "No such command" <<"\x1b[0m" << std::endl;
        return;
    }

    if (++argCount < itr->second.argCount)
    {
        std::cout <<"\x1b[31;1m"<< "Insuficient arguments"<<"\x1b[0m" << std::endl;
        return;
    }
    (*(itr->second.handler))(args, client);
}

void msgCommand(std::string arguments, IRCClient* client)
{
    std::string to = arguments.substr(0, arguments.find(" "));
    std::string text = arguments.substr(arguments.find(" ") + 1);
    if (to[0] == '#')
    {
        std::cout << "To " << "\x1b[35;2m" << to << "\x1b[0m" << ":" + text << std::endl;
    }
    else
    {
        std::cout << "To " << "\x1b[34;2m" << "<" << to << ">" << "\x1b[0m" << ":" + text << std::endl;
    }
    client->SendIRC("PRIVMSG " + to + " :" + text);
}

void joinCommand(std::string channel, IRCClient* client)
{
    if (channel[0] != '#')
        channel = "#" + channel;

    client->SendIRC("JOIN " + channel);
}

void partCommand(std::string channel, IRCClient* client)
{
    if (channel[0] != '#')
        channel = "#" + channel;

    client->SendIRC("PART " + channel);
}

void invtCommand(std::string arguments, IRCClient* client)
{
    std::string name = arguments.substr(0, arguments.find(" "));
    std::string channel = arguments.substr(arguments.find(" ") + 1);
    if (channel[0] != '#')
        channel = "#" + channel;
    //std::cout << "You invited " + name + " into channel " + channel << std::endl;
    client->SendIRC("INVITE " + name + " " + channel);
}

void kickCommand(std::string arguments, IRCClient* client)
{
    std::string channel = arguments.substr(0, arguments.find(" "));
    std::string nick = arguments.substr(arguments.find(" ") + 1);
    if (channel[0] != '#')
        channel = "#" + channel;
    //std::cout << "You kicked " << nick << " from channel " << channel << std::endl;
    client->SendIRC("KICK " + channel + " " + nick);
}

void quitCommand(std::string , IRCClient* client)
{
    client->SendIRC("QUIT");
}


void helpCommand(std::string, IRCClient*)
{
    std::cout << "\x1b[37;4m"<<"Available commands:" <<"\x1b[0m"<< std::endl << std::endl;
    std::cout << "Join channel:             /join <channel_name>" << std::endl;
    std::cout << "Leave channel:            /part <channel_name>" << std::endl;
    std::cout << "Write a message:          /msg <msg_target> <text to be sent>" << std::endl;
    std::cout << "Leave channel:            /part <channel_name>" << std::endl;
    std::cout << "Leave network:            /quit [<quit_message>]" << std::endl;
    std::cout << "Invite user to channel:   /invt <nick_name> <channel>" << std::endl;
    std::cout << "Remove user from channel: /kick <channel> <nick_name>" << std::endl;
}
