#include <string>
#include <iostream>
#include <cstdlib>

#include "common.h"
#include "logger/Logger.h"
#include "comm/FifoLectura.h"
#include "parser/Parser.h"

int main(int argc, char* argv[]) {
	const std::string me = __FILE__ ":main";
	
	// Para debuggear - borrar despues
	/*std::cout << "Argc: " << argc << std::endl;
	argc--;
	while(argc >= 0) {
		std::cout << "Argv[" << argc << "]: " << argv[argc] << std::endl;
		argc--;
	}*/

	// Parsear linea de comandos
	Parser parser;
	if(!parser.parse(argc, argv)) {
		std::cerr << me << "-- Error parseando argv\n";
		exit(2);
	}

	// Init Logger
	if(parser.debugMode()) {
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
