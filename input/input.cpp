// Pequeño programa para generar el input del programa principal
// Establece el standard de input para el programa principal
// Via fifo, se escriben 'autos'. Cada auto es un int
// El programa principal lee el otro end del fifo
// Este programa se encarga de generar dicho stream, poniendo los delays entre cada auto como sea necesario
// Como parametro toma la cantidad de autos a generar
// Un ejemplo de salida (pasada a ascii) seria
// ./input 4
// 1 2 3 4(EOF)

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sstream>

#include "comm/FifoEscritura.h"
#include "common.h"
#include "logger/Logger.h"

// Constantes de configuracion
// Minimo y maximo delay entre la llegada de autos consecutivos
const int min_delay_default = 1;
const int max_delay_default = 5;
const int cant_autos_default = 10;

int main(int argc, const char* argv[]) {
	
	if(argc == 2 && std::string(argv[1]) == "-h") {
		std::cout << "input <cant_autos> <min_delay> <max_delay>\n";
		return 1;
	}
	
	// Cargamos los parametros de argv o default
	const int cant_autos = (argc >= 2 ? std::atoi(argv[1]) : cant_autos_default);
	const int min_delay = (argc >= 3 ? std::atoi(argv[2]) : min_delay_default);
	const int max_delay = (argc >= 4 ? std::atoi(argv[3]) : max_delay_default);
	// Breve chequeo de params
	if(cant_autos <= 0 || min_delay <= 0 || max_delay <= 0 || min_delay >= max_delay) {
		std::cerr << "Invalid arguments!\n";
		return 2;
	}

	// Init Logger
	const std::string me = __FILE__ ":main";
	Logger::initialize(logFile.c_str(),Logger::LOG_DEBUG);

	Logger::debug("Init input", me);

	// rand seed
	std::srand(std::time(NULL));

	// Setup del fifo
	FifoEscritura canal(fifoInputJefe);
	canal.abrir();

	// Loop para generar
	for(int current=0;current < cant_autos;current++) {
		// sleep hasta que llega
		const int sleep_interval = (rand() % (max_delay - min_delay)) + min_delay;
		sleep(sleep_interval);
		// escritura al fifo
		canal.escribir(static_cast<const void*>(&current), sizeof(current));
		// debug write
		std::stringstream msg;
		msg << current;
		Logger::debug(std::string("Write al pipe: ") + msg.str(), me);
	}
	
	// Close del fifo y cleanup del archivo
	canal.cerrar();

	return 0;
}
