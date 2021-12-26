#pragma once
#include <ncurses.h>

class Ncurses
{
public:
	void DisplayChat(WINDOW* window);
	void CreateWin(int y, int x, int start_y, int start_x)
	{
		window = newwin(y, x, start_y, start_x);
	};

    void Output(WINDOW* window, std::vector<std::string>* responses, std::string data) {
        responses->push_back(data);
        int response_counter = 1;
        int counter = 0;
        int diff = 0;

        if ((int)responses->size() >= window->_maxy - 3) {
            diff = responses->size() - window->_maxy + 3;
        }

        for (std::string& response : *responses) {
            if (counter >= diff) {
                int color_print = 0;

                response_counter++;
                wattron(window, COLOR_PAIR(color_print));
                mvwprintw(window, response_counter, 6, response.c_str());
                wattroff(window, COLOR_PAIR(color_print));
            }
            counter++;
        }
        DisplayChat(window);
    };

	WINDOW* window;
	std::vector<std::string> *responses;
};