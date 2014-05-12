CC=g++
CFLAGS=-Wall -pedantic -pedantic-errors -g -ggdb -DDEBUG
LDFLAGS=
BINF=bin
HEADER_DIR=headers

SHARED_SOURCES := $(shell find src/*/ -name '*.cpp')
SHARED_OBJ=$(SHARED_SOURCES:.cpp=.o)

INPUT_SOURCES=input/input.cpp
INPUT_OBJ=$(INPUT_SOURCES:.cpp=.o)
INPUT_BIN=$(BINF)/input

ESTACION_SOURCES=src/estacion.cpp
ESTACION_OBJ=$(ESTACION_SOURCES:.cpp=.o)
ESTACION_BIN=$(BINF)/estacion

JEFE_SOURCES=src/jefeEstacion.cpp
JEFE_OBJ=$(JEFE_SOURCES:.cpp=.o)
JEFE_BIN=$(BINF)/jefeEstacion

EMPLEADO_SOURCES=src/empleado.cpp
EMPLEADO_OBJ=$(EMPLEADO_SOURCES:.cpp=.o)
EMPLEADO_BIN=$(BINF)/empleado

ADMINISTRADOR_SOURCES=src/administrador.cpp
ADMINISTRADOR_OBJ=$(ADMINISTRADOR_SOURCES:.cpp=.o)
ADMINISTRADOR_BIN=$(BINF)/administrador

all: estacion jefeEstacion input empleado administrador

estacion: $(ESTACION_OBJ) $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) $(ESTACION_OBJ) -o $(ESTACION_BIN) -I$(HEADER_DIR)

jefeEstacion: $(JEFE_OBJ) $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) $(JEFE_OBJ) -o $(JEFE_BIN) -I$(HEADER_DIR)

input: $(INPUT_OBJ) $(SHARED_OBJ)
	$(CC) $(INPUT_OBJ) $(SHARED_OBJ) -o $(INPUT_BIN)

empleado: $(EMPLEADO_OBJ) $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) $(EMPLEADO_OBJ) -o $(EMPLEADO_BIN) -I$(HEADER_DIR)
	
administrador: $(ADMINISTRADOR_OBJ) $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) $(ADMINISTRADOR_OBJ) -o $(ADMINISTRADOR_BIN) -I$(HEADER_DIR)

.cpp.o:
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	$(RM) $(ESTACION_OBJ) $(ESTACION_BIN) $(JEFE_OBJ) $(JEFE_BIN) $(INPUT_OBJ) $(INPUT_BIN) $(ADMINISTRADOR_OBJ) $(ADMINISTRADOR_BIN) $(EMPLEADO_OBJ) $(EMPLEADO_BIN) $(SHARED_OBJ)
