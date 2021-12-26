#include <iostream>
#include <ncurses.h>
#include "thread.hpp"
#include "console_handler.hpp"
#include "ncurses.hpp"

ThreadReturn inputThread(void* client)
{
    std::string command;
    ConsoleCommandHandler commandHandler;

    commandHandler.AddCommand("msg", 2, &msgCommand);
    commandHandler.AddCommand("join", 1, &joinCommand);
    commandHandler.AddCommand("part", 1, &partCommand);
    commandHandler.AddCommand("help", 0, &helpCommand);
    commandHandler.AddCommand("invt", 2, &invtCommand);
    commandHandler.AddCommand("kick", 2, &kickCommand);

    while (true)
    {
        getline(std::cin, command);
        if (command == "")
            continue;

        if (command[0] == '/')
            commandHandler.ParseCommand(command, (IRCClient*)client);
        else
            ((IRCClient*)client)->SendIRC(command);

        if (command == "quit")
            break;
    }

    pthread_exit(NULL);
}

 void Ncurses::DisplayChat(WINDOW *window){
  const int column{1}; 
  int row{1}; 
  
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_GREEN, COLOR_WHITE);
  init_pair(8, COLOR_WHITE, COLOR_GREEN);
  init_pair(9, COLOR_WHITE, COLOR_BLACK);
	
  wclear(window);
        
  box(window, 0, 0);   
  
  wattron(window, A_BOLD);
  wattron(window, COLOR_PAIR(4));
  mvwprintw(window, row++, column, "Chat:");
  wattroff(window, COLOR_PAIR(4));
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, window->_maxy - 1, column, "send Message"); 
  wattroff(window, COLOR_PAIR(2));
  wattroff(window, A_BOLD);   

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
    
    //start ncurses
    initscr();
    cbreak();
    echo();
    start_color();

    std::vector<std::thread> threads;
    //std::vector<std::string> responses ; 

    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);
    //int text_x{1}, text_y{1};
    Ncurses win;
    win.CreateWin(y_max, x_max, 0, 0);
    //WINDOW *window = newwin(y_max, x_max, 0, 0);
    viewwin view;
    for (int i=0; i < 4; i++){
        view._fields.emplace_back("");
    }
    // Start the input thread
    Thread thread;
    thread.Start(&inputThread, &client);

    if (client.InitSocket())
    {
        printw("Socket initialized. Connecting...");


        if (client.Connect(host, port))
        {
            printw("Connected. Loggin in...");

            if (client.Login(nick, user))
            {
                std::cout << "Logged." << std::endl;
                while (client.Connected()){
                    win.DisplayChat(win.window);
                    std::string message_text; 
  
  		    std::string checker = ""; 
  		    for (std::string &text : view._fields){
    			message_text += text + " "; 
    			checker += text; 
    			text.clear(); 
  		    }
  		    refresh();
 		    move(win.window->_maxy - 1, 16);
                    client.ReceiveData();
                    wrefresh(win.window);
                    refresh();
                    }
                    
            }

            if (client.Connected())
                client.Disconnect();

            std::cout << "Disconnected." << std::endl;
        }
    }
}
