#include <iostream>
#include <ncurses.h>
#include "client.h"
#include "thread.h"
#include "console_handler.h"

ThreadReturn inputThread(void* client)
{
    std::string command;
    ConsoleCommandHandler commandHandler;

    commandHandler.AddCommand("msg", 2, &msgCommand);
    commandHandler.AddCommand("join", 1, &joinCommand);
    commandHandler.AddCommand("part", 1, &partCommand);
    commandHandler.AddCommand("help", 0, &helpCommand);
    commandHandler.AddCommand("invt", 2, &invtCommand);
    commandHandler.AddCommand("kick", 2, &kickCommand);
    commandHandler.AddCommand("quit", 1, &quitCommand);

    while (true)
    {
        getline(std::cin, command);
        if (command == "")
            continue;

        if (command[0] == '/')
            commandHandler.ParseCommand(command, (IRCClient*)client);
        else
            ((IRCClient*)client)->SendIRC(command);

        if (command == "quit")
            break;
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    /*
    if (argc < 3)
    {
        std::cout << "Insuficient parameters: host port [nick] [user]" << std::endl;
        return 1;
    }

    char* host = argv[1];
    int port = atoi(argv[2]);
    std::string nick("Pupa");
    std::string user("PUPA");

    if (argc >= 4)
        nick = argv[3];
    if (argc >= 5)
        user = argv[4];
    */
    char* host = (char*) malloc(5);
    char* ports = (char*) malloc(5);
    std::string nick;
    std::string user;
    std::cout << "Enter server host: ";
    std::cin >> host;
    std::cout << "Enter server port: ";
    std::cin >> ports;
    std::cout << "Enter nickname: ";
    std::cin >> nick;
    std::cout << "Enter username: ";
    std::cin >> user;

    int port = atoi(ports);
    
    IRCClient client;

    // Start the input thread
    Thread thread;
    thread.Start(&inputThread, &client);

    if (client.InitSocket())
    {
            std::cout << "Socket initialized. Connecting..." << std::endl;

            if (client.Connect(host, port))
            {
                std::cout << "Connected. Loggin in..." << std::endl;

                if (client.Login(nick, user))
                {
                    std::cout << "Logged." << std::endl;
                    while (client.Connected())
                    {
                        client.ReceiveData();
                    }

                }

                if (client.Connected())
                    client.Disconnect();
                std::cout << "Disconnected." << std::endl;
            }
    }
}