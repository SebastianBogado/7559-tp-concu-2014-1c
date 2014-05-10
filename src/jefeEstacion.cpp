#include <string>
#include <iostream>
#include <cstdlib>

#include "common.h"
#include "logger/Logger.h"
#include "comm/FifoLectura.h"
#include "parser/Parser.h"
#include "comm/ArgHelper.h"

int main(int argc, char* argv[]) {
	const std::string me = __FILE__ ":main";
	
	// Para debuggear - borrar despues
	/*std::cout << "Argc: " << argc << std::endl;
	for(int c = 0;c < argc;c++) {
		std::cout << "Argv[" << c << "]: " << argv[c] << std::endl;
	}*/

	// Parsear los argumentos que nos manda el padre
	bool debugMode;
	int cantEmpleados, cantAutos, myId;
	
	if(!ArgHelper::decode(argc, argv, &debugMode, &cantEmpleados, &cantAutos, &myId)) {
		std::cerr << "Error - invalid parameters passed to process " << argv[0] << std::endl;
		exit(1);
	}

	// Init Logger
	if(debugMode) {
		Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	}else{
		Logger::initialize(logFile.c_str(), Logger::LOG_WARNING);
	}
	
	Logger::debug("Init jefeEstacion", me);

	// Setup del pipe de input
	FifoLectura canal(fifoInputJefe);
	const int buffer_size = sizeof(int);
	char buffer[buffer_size];
	canal.abrir();
	bool leyendo = true;
	while(leyendo) {
		ssize_t bytes = canal.leer(static_cast<void*>(buffer), buffer_size);
		if(bytes == 0) {
				Logger::debug("Sali del input loop por EOF", me);
				leyendo = false;
		}else if(bytes < 0) {
				Logger::debug("Sali del input loop por error de lectura", me);
				leyendo = false;
		}else{
			int received_id = static_cast<int>(*buffer);
			std::stringstream ss;
			ss << "Leí del fifo: " << received_id;
			Logger::debug(ss.str(), me);
		}
	}
	
	canal.cerrar();
	
}
