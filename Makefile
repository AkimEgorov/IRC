CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lpthread -lncurses
SOURCES=client.cpp console_handler.cpp handler.cpp main.cpp socket.cpp thread.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=irc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
