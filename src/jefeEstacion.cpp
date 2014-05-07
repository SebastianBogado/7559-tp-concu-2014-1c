#include <string>
#include <iostream>

#include "common.h"
#include "logger/Logger.h"
#include "comm/FifoLectura.h"

int main(int argc, char* argv[]) {

	// Init Logger
	const std::string me = __FILE__ ":main";
	Logger::initialize(logFile,Logger::LOG_DEBUG);

	Logger::debug("Init jefeEstacion", me);

	// Setup del pipe de input
	FifoLectura canal(fifoInputJefe);
	const int buffer_size = sizeof(int);
	char buffer[buffer_size];
	canal.abrir();
	for(;;) {
		ssize_t bytes = canal.leer(static_cast<void*>(buffer), buffer_size);
		if(bytes == 0) {
				Logger::debug("Sali del input loop por EOF", me);
				break;
		}else if(bytes < 0) {
				Logger::debug("Sali del input loop por error de lectura", me);
				break;
		}
		int received_id = static_cast<int>(*buffer);
		std::stringstream ss;
		ss << "Leí del fifo: " << received_id;
		Logger::debug(ss.str(), me);
	}
	
	canal.cerrar();
	
}
