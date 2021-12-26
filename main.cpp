#include <iostream>
#include "client.hpp"
#include "thread.hpp"
#include "console_handler.hpp"
#include "interface.hpp"

ThreadReturn inputThread(void* client)
{
    char command[200];
    ConsoleCommandHandler commandHandler;

    commandHandler.AddCommand("msg", 2, &msgCommand);
    commandHandler.AddCommand("join", 1, &joinCommand);
    commandHandler.AddCommand("part", 1, &partCommand);
    commandHandler.AddCommand("help", 0, &helpCommand);
    commandHandler.AddCommand("invt", 2, &invtCommand);
    commandHandler.AddCommand("kick", 2, &kickCommand);

    while (true)
    {
        wgetstr(stdscr,(command));
        if (strcmp(command,"")==0)
            continue;

        if (command[0] == '/')
            commandHandler.ParseCommand(command, (IRCClient*)client);
        else
            ((IRCClient*)client)->SendIRC(command);

        if (strcmp(command,"quit")==0)
            break;
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printw("Insuficient parameters: host port [nick] [user]");
        return 1;
    }

    char* host = argv[1];
    int port = atoi(argv[2]);
    std::string nick("MyIRCClient");
    std::string user("IRCClient");

    if (argc >= 4)
        nick = argv[3];
    if (argc >= 5)
        user = argv[4];

    IRCClient client;

    // Start the input thread
    Thread thread;
    thread.Start(&inputThread, &client);

    if (client.InitSocket())
    {
        printw("Socket initialized. Connecting...");

        if (client.Connect(host, port))
        {
            printw("Connected. Loggin in...");

            if (client.Login(nick, user))
            {
                printw("Logged.");
                Display(&client);
            }

            if (client.Connected())
                client.Disconnect();

            printw("Disconnected.");
        }
    }
}
