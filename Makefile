SRC = ./src
BIN = ./bin
CC = gcc
POSTFLAGS = -fsanitize=address
EXECUTABLE = terminal
SOURCES = main.c

.PHONY: clean $(EXECUTABLE) build

$(EXECUTABLE): 
	if [ ! -d $(BIN) ] then
	mkdir $(BIN)
	fi
	$(CC) -o $(BIN)/$(EXECUTABLE) $(SRC)/$(SOURCES) $(POSTFLAGS)

build:
	if [ ! -d $(BIN) ] then
	mkdir $(BIN)
	fi
    $(CC) -g -o $(BIN)/$(EXECUTABLE) $(SRC)/$(SOURCES) $(POSTFLAGS)

clean:
	rm -rf $(BIN)/$(EXECUTABLE)
