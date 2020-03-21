SRC = ./src
BIN = ./bin
CC = gcc
POSTFLAGS = -fsanitize=address
EXECUTABLE = terminal
SOURCES = main.c

.PHONY: clean $(EXECUTABLE)

$(EXECUTABLE): 
	$(CC) -g -o $(BIN)/$(EXECUTABLE) $(SRC)/$(SOURCES) $(POSTFLAGS)

clean:
	rm -rf $(BIN)/$(EXECUTABLE)
