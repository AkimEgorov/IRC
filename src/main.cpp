#include <iostream>
#include <signal.h>
#include <cstdlib>
#include <map>
#include <algorithm>
#include "client.h"
#include "thread.h"

volatile bool running;

void signalHandler(int signal)
{
    running = false;
}

ThreadReturn inputThread(void* client)  //  new stream for sending messages
{
    std::string command;

    while (true)
    {
        getline(std::cin, command);
        ((IRCClient*)client)->SendIRC(command);

        if (command == "quit")  //  closing the stream
            break;
    }

    pthread_exit(NULL);  
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Insuficient parameters: host port [nick] [user]" << std::endl;
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
    thread.Start(&inputThread, &client);  //  creating a new stream for sending messages

    if (client.InitSocket())
    {
        std::cout << "Socket initialized. Connecting..." << std::endl;

        if (client.Connect(host, port))
        {
            std::cout << "Connected. Loggin in..." << std::endl;

            if (client.Login(nick, user))
            {
                std::cout << "Logged." << std::endl;
                running = true;
                signal(SIGINT, signalHandler);

                while (client.Connected() && running)
                    client.ReceiveData();
            }

            if (client.Connected())
                client.Disconnect();

            std::cout << "Disconnected." << std::endl;
        }
    }
}





