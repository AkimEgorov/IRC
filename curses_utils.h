
#ifndef UTILS_H
#define UTILS_H
#define FIELD_MAX_CHARS 32

#include <ncurses.h>
#include <string>
#include <vector>c

struct viewwin {
    std::vector<std::string> _fields;
};


    void Display();
    void DisplayMessages(WINDOW *window, viewwin *view,std::vector<std::string> responses);
    void DisplayUsers(WINDOW *window);
    void TextBox(viewwin *view);
#endif
