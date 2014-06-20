CC=g++
CFLAGS=-Wall -pedantic -pedantic-errors -g -ggdb -DDEBUG
ROOT=../../
HEADER_DIR=$(ROOT)headers

SHARED_SOURCES := $(shell find ../../src/*/ -name '*.cpp')
SHARED_OBJ=$(SHARED_SOURCES:.cpp=.o)

TEST_SOURCES=Cliente.cpp Servidor.cpp
TEST_OBJ=$(TEST_SOURCES:.cpp=.o)
TEST_BIN=test

test: $(TEST_OBJ) $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) $(TEST_OBJ) -o $(TEST_BIN) -I$(HEADER_DIR)

.cpp.o:
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	$(RM) $(TEST_OBJ) $(TEST_BIN) $(SHARED_OBJ)