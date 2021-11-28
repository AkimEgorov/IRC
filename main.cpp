#include <iostream>
#include "client.h"
#include "thread.h"

ThreadReturn inputThread(void* client)
{
    std::string command;

    while (true)
    {
        getline(std::cin, command);
        ((IRCClient*)client)->SendIRC(command);

        if (command == "quit")
            break;
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    char* host;
    char* ports;
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
                    client.ReceiveData();
            }

            if (client.Connected())
                client.Disconnect();

            std::cout << "Disconnected." << std::endl;
        }
    }
}