#include <algorithm>
#include <chrono>
#include <form.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <string.h>
#include <memory>
#include <sstream>
#include <mutex>
#include "interface.hpp"

static std::mutex mtx; 
 
void DisplayChat(WINDOW *window){
  const int column{1}; 
  int row{1}; 
  
  wattron(window, A_BOLD);
  wattron(window, COLOR_PAIR(4));
  mvwprintw(window, row++, column, "Chat:");
  wattroff(window, COLOR_PAIR(4));
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, window->_maxy - 1, column, "send Message"); 
  wattroff(window, COLOR_PAIR(2));
  wattroff(window, A_BOLD);
    

}

void DisplayMessages(WINDOW *window, viewwin *view,std::vector<std::string> *responses ){
  std::string message_text; 
  
  std::string checker = ""; 
  mtx.lock();
  for (std::string &text : view->_fields){
    message_text += text + " "; 
    checker += text; 
    text.clear(); 
  }
  mtx.unlock();
  refresh();
  move(window->_maxy - 1, 16);                 
  char str[100];
  int ch;
  getstr(str);
  refresh();
  
  responses->push_back(str);
 
  int response_counter=1;
  int counter=0;
  int diff =0;
  
  if (responses->size() >= window->_maxy - 3) {
    diff = responses->size() - window->_maxy + 3; 
  }
  for (std::string &response : *responses) {
     if (counter >= diff) {
      int color_print = 0;

      response_counter++;
      wattron(window, COLOR_PAIR(color_print));
      std::string response_text;
      mvwprintw(window, response_counter, 6, response.c_str());
      wattroff(window, COLOR_PAIR(color_print));
    }
    counter++;
  }
}

void Display(){
    initscr();
    cbreak();
    echo();
    start_color();

    std::vector<std::thread> threads;

    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);
    int text_x{1}, text_y{1};

    WINDOW *chat_window = newwin(y_max * text_y, x_max * text_x, 0, 0);
    viewwin view;
    for (int i=0; i < 4; i++){
        view._fields.emplace_back("");
    }
    
    std::vector<std::string> responses ; 
    while (TRUE) {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_GREEN, COLOR_WHITE);
        init_pair(8, COLOR_WHITE, COLOR_GREEN);
        init_pair(9, COLOR_WHITE, COLOR_BLACK);
	
        wclear(chat_window);
        
        box(chat_window, 0, 0);
        
        DisplayChat(chat_window);
        refresh();
        DisplayMessages(chat_window,  &view, &responses);
	
	mtx.lock();
        wrefresh(chat_window);
        refresh();
        mtx.unlock();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    }
    threads.front().join();
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) { t.join(); });
    endwin();
}

int main(){
    Display();
}
