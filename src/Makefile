CC=g++
CFLAGS=-c -g -Wno-write-strings
LDFLAGS=
LIBS=-ltelldus-core -lcurl -levent
SOURCES=main.cpp listen.cpp events.cpp action.cpp config.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=homecon

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(EXECUTABLE)