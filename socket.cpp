#include <cstring>
#include <fcntl.h>
#include "socket.h"

#define MAXDATASIZE 4096

bool IRCSocket::Init()
{
    if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        std::cout << "Socket error." << std::endl;
        return false;
    }

    int on = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char const*)&on, sizeof(on)) == -1)
    {
        std::cout << "Invalid socket." << std::endl;
        return false;
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);
    fcntl(_socket, F_SETFL, O_ASYNC);
    return true;
}


bool IRCSocket::Connect(char const* host, int port)
{
    hostent* he;

    if (!(he = gethostbyname(host)))
    {
        std::cout << "Could not resolve host: " << host << std::endl;
        return false;
    }

    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((const in_addr*)he->h_addr);
    memset(&(addr.sin_zero), '\0', 8);

    if (connect(_socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cout << "Could not connect to: " << host << std::endl;
        closesocket(_socket);
        return false;
    }

    _connected = true;

    return true;
}

void IRCSocket::Disconnect()
{
    if (_connected)
    {
        closesocket(_socket);
        _connected = false;
    }
}

bool IRCSocket::SendData(char const* data)
{
    if (_connected)
        if (send(_socket, data, strlen(data), 0) == -1)
            return false;

    return true;
}

std::string IRCSocket::ReceiveData()
{
    char buffer[MAXDATASIZE];

    memset(buffer, 0, MAXDATASIZE);

    int bytes = recv(_socket, buffer, MAXDATASIZE - 1, 0);

    if (bytes > 0)
        return std::string(buffer);
    else
        Disconnect();

    return "";
}