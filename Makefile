CC=g++
CFLAGS=-Wall -pedantic -pedantic-errors -g -ggdb -DDEBUG
LDFLAGS=
MAIN_SOURCES := $(shell find src/ -name '*.cpp')
MAIN_OBJ=$(MAIN_SOURCES:.cpp=.o)
INPUT_SOURCES=input/input.cpp
INPUT_OBJ=$(INPUT_SOURCES:.cpp=.o)
BINF=bin
MAIN_BIN=$(BINF)/estacion
INPUT_BIN=$(BINF)/input
HEADER_DIR=headers

all: estacion input

estacion: $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) -o $(MAIN_BIN) -I$(HEADER_DIR)

input: $(INPUT_OBJ)
	$(CC) $(INPUT_OBJ) -o $(INPUT_BIN)

.cpp.o:
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	$(RM) $(INPUT_OBJ) $(INPUT_BIN) $(MAIN_OBJ) $(MAIN_BIN)
