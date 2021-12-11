#include <iostream>
#include "socket.h"
#include "client.h"
#include "handler.h"

// cheecks socket initialization
bool IRCClient::InitSocket()
{
    return _socket.Init();
}

// cheecks socket connection to the server
bool IRCClient::Connect(char* host, int port)
{
    return _socket.Connect(host, port);
}

// disconnects client from the server
void IRCClient::Disconnect()
{
    _socket.Disconnect();
}

// transforms string into char const* and checks data sending
bool IRCClient::SendIRC(std::string data)
{
    data.append("\n");
    return _socket.SendData(data.c_str());
}

// registers a user on the network
bool IRCClient::Login(std::string nick, std::string user, std::string password)
{
    _nick = nick;
    _user = user;

    if (SendIRC("HI"))
    {
        if (SendIRC("NICK " + nick))
            if (SendIRC("USER " + user + " 8 * :IRC Client"))
                return true;
    }

    return false;
}

// process the received data
void IRCClient::ReceiveData()
{
    std::string buffer = _socket.ReceiveData();

    std::string line;
    std::istringstream iss(buffer);
    while (getline(iss, line))
    {
        if (line.find("\r") != std::string::npos)
            line = line.substr(0, line.size() - 1);
        Parse(line);
    }
}

// parcer for messages
void IRCClient::Parse(std::string data)
{
    std::string original(data);
    IRCCommandPrefix cmdPrefix;

    // checks if command has prefix
    if (data.substr(0, 1) == ":")
    {
        // if yes, parces and get the prefix
        cmdPrefix.prefix_parse(data);
        data = data.substr(data.find(" ") + 1);
    }

//  looks for a command
    std::string command = data.substr(0, data.find(" "));
    if (data.find(" ") != std::string::npos)
        data = data.substr(data.find(" ") + 1);
    else
        data = "";

    std::vector<std::string> parameters;

    // checks if command has parameters
    if (data != "")
    {
        if (data.substr(0, 1) == ":")
            parameters.push_back(data.substr(1));
        else
        {
            // if yes, parces and get patameters
            size_t pos1 = 0, pos2;
            while ((pos2 = data.find(" ", pos1)) != std::string::npos)
            {
                parameters.push_back(data.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
                if (data.substr(pos1, 1) == ":")
                {
                    parameters.push_back(data.substr(pos1 + 1));
                    break;
                }
            }
            if (parameters.empty())
                parameters.push_back(data);
        }
    }

    // disconnets client from server if received an ERROR message 
    if (command == "ERROR")
    {
        std::cout << original << std::endl;
        Disconnect();
        return;
    }

    // replies to the server to the message PING
    if (command == "PING")
    {
        //std::cout << "Pong!" << std::endl;
        SendIRC("PONG :" + parameters.at(0));
        return;
    }

    // create an object of class IRCMessage
    IRCMessage ircMessage(command, cmdPrefix, parameters);

    // handles commands
    int commandIndex = GetCommandHandler(command);
    if (commandIndex < N_IRC_CMD)
    {
        cmdHandler& cmdHandler = ircCommandTable[commandIndex];
        (this->*cmdHandler.handler)(ircMessage);
    }
    
}