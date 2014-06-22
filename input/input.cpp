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

#include "comm/ColaConPrioridad.hpp"
#include "common.h"
#include "logger/Logger.h"

// Constantes de configuracion
// Minimo y maximo delay entre la llegada de autos consecutivos
const int min_delay_default = 1;
const int max_delay_default = 5;
const int cant_autos_default = 10;
const float porcentaje_vip_default = 0.1;

int main(int argc, const char* argv[]) {
	
	if(argc == 2 && std::string(argv[1]) == "-h") {
		std::cout << "input <cant_autos> <min_delay> <max_delay> <porcentaje_vip>\n";
		return 1;
	}
	
	// Cargamos los parametros de argv o default
	const int cant_autos = (argc >= 2 ? std::atoi(argv[1]) : cant_autos_default);
	const int min_delay = (argc >= 3 ? std::atoi(argv[2]) : min_delay_default);
	const int max_delay = (argc >= 4 ? std::atoi(argv[3]) : max_delay_default);
	const float porcentaje_vip = (argc >= 5 ? std::atof(argv[4]) : porcentaje_vip_default);

	// Breve chequeo de params
	if(cant_autos <= 0 || min_delay <= 0 || max_delay <= 0 || min_delay >= max_delay || porcentaje_vip > 1 || porcentaje_vip < 0) {
		std::cerr << "Invalid arguments!\n";
		return 2;
	}

	try {

		// Init Logger
		const std::string me = __FILE__ ":main";
		Logger::initialize(logFile.c_str(),Logger::LOG_DEBUG);

		Logger::debug("Init input", me);

		// rand seed
		std::srand(std::time(NULL));

		// Setup cola con prioridad
		ColaConPrioridad<automovil> cola = ColaConPrioridad<automovil> ( colaInputJefe,colaInputJefeKey );

		// Loop para generar
		for(int current=0;current < cant_autos;current++) {
			// sleep hasta que llega
			const int sleep_interval = (rand() % (max_delay - min_delay)) + min_delay;
			sleep(sleep_interval);
			bool esVip = ((rand() % 100) / 100.0) <= porcentaje_vip;
			automovil automovil = {current, esVip};
			// escritura a la cola
			cola.escribir(automovil, (automovil.esVip? 1 : 0));
			// debug write
			std::stringstream msg;
			msg << current;
			if (esVip) msg << " (VIP)";
			Logger::debug(std::string("Write a la cola: ") + msg.str(), me);
		}

		// Escribir mensaje de fin de input
		cola.escribir( automovilFinal );

	}catch(std::string& err) {
		std::cerr << "Oops!: " << err << "\n";
		return 1;
	}

	return 0;
}
