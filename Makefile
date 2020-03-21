SRC = ./src
BIN = ./bin
CC = gcc
POSTFLAGS = -fsanitize=address
EXECUTABLE = terminal
SOURCES = main.c

.PHONY: clean $(EXECUTABLE)

$(EXECUTABLE): 
	mkdir $(BIN)
	$(CC) -o $(BIN)/$(EXECUTABLE) $(SRC)/$(SOURCES) $(POSTFLAGS)

clean:
	rm -rf $(BIN)/$(EXECUTABLE)
	rmdir $(BIN)