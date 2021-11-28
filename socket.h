#ifndef _SOCKET_H
#define _SOCKET_H

#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define closesocket(s) close(s)
#define close(s)
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

class IRCSocket
{
public:
    bool Init();  //  socket creation

    bool Connect(char const* host, int port);//  server connection
    void Disconnect(); //  Disconnect from server

    bool Connected() { return _connected; }; //  connected to the server

    bool SendData(char const* data);
    std::string ReceiveData();

private:
    int _socket;

    bool _connected;
};
#endif
