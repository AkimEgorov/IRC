#include <cstring>
#include <fcntl.h>
#include "socket.h"

#define MAXDATASIZE 4096

bool IRCSocket::Init()
{
    if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)  //  check for socket creation
    {
        std::cout << "Socket error." << std::endl;
        return false;
    }

    int on = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char const*)&on, sizeof(on)) == -1)  //  This enables get around those "Address already in use"
    {
        std::cout << "Invalid socket." << std::endl;
        return false;
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);  //  makes the socket non-blocking  
    fcntl(_socket, F_SETFL, O_ASYNC);  //  The bit that enables asynchronous input mode.
    return true;
}


bool IRCSocket::Connect(char const* host, int port)  //  server connection
{
    hostent* he;

    if (!(he = gethostbyname(host)))  //  getting hostname
    {
        std::cout << "Could not resolve host: " << host << std::endl;
        return false;
    }

    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((const in_addr*)he->h_addr);
    memset(&(addr.sin_zero), '\0', 8);

    if (connect(_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)  //  establishing a connection to the server
    {
        std::cout << "Could not connect to: " << host << std::endl;
        closesocket(_socket);
        return false;
    }

    _connected = true;

    return true;
}

void IRCSocket::Disconnect() //  disconnect from server
{
    if (_connected)
    {
        closesocket(_socket);
        _connected = false;
    }
}

bool IRCSocket::SendData(char const* data)  //  sending a message 
{
    if (_connected)
        if (send(_socket, data, strlen(data), 0) == -1)
            return false;

    return true;
}

std::string IRCSocket::ReceiveData()  //  receive message 
{
    char buffer[MAXDATASIZE];

    memset(buffer, 0, MAXDATASIZE);

    int bytes = recv(_socket, buffer, MAXDATASIZE - 1, 0);

    if (bytes > 0)  //  check for messages from the server
        return std::string(buffer);
    else
        Disconnect();

    return "";
}