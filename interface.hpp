#pragma once
#define FIELD_MAX_CHARS 32

#include "/usr/include/curses.h"
#include <string>
#include <vector>



struct viewwin {
    std::vector<std::string> _fields;
};


    void Display();
    void DisplayMessages(WINDOW *window, viewwin *view,std::vector<std::string> responses);
    void DisplayUsers(WINDOW *window);
    void TextBox(viewwin *view);
