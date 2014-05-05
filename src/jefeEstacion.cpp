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
	const int buffer_size = 4;
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
		std::string msg = buffer;
		msg.resize(bytes);
		Logger::debug(std::string("Leí del fifo: ") + msg, me);
	}
	
	canal.cerrar();
	
}
