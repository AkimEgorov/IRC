#pragma once
#define FIELD_MAX_CHARS 32

#include "/usr/include/curses.h"
#include <string>
#include <cstring>
#include <vector>
#include "client.hpp"
#include "thread.hpp"
#include "console_handler.hpp"



struct viewwin {
    std::vector<std::string> _fields;
};


    void Display(IRCClient *client);
    void DisplayChat();
    void DisplayMessages(WINDOW *window, viewwin *view,std::vector<std::string> responses);

