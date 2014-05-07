/*
*
*	TP 1 Técnicas de programación concurrentes I
*	Integrantes:
*		Bogado, Sebastián
*		Martty, Juan
*		Pereira, Fernando
*
*/

#include <cstdlib>

#include "logger/Logger.h"
#include "common.h"

int main(int argc, char* argv[]) {
	// Init Logger
	Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);

	unsigned int idEmpleado, cantEmpleados;
	std::string idEmpleadoStr, cantEmpleadosStr,
		me = __FILE__;


	// Parseo cabeza de args
	if ( argc > 1 ) {
		idEmpleadoStr = argv[1];
		me += "." + idEmpleadoStr + ":main";
		idEmpleado = atoi(idEmpleadoStr.c_str());
		Logger::debug("Parámetro parseado 'idEmpleado': " + idEmpleadoStr, me);
	}  else {
		me += ":main";
		Logger::error("Argumentos insuficientes. Se espera \n"
					  "$ ./empleado <idEmpleado> <cantEmpleados>", me);
		return 1;
	}

	if ( argc > 2 ) {
		cantEmpleadosStr = argv[2];
		cantEmpleados = atoi(cantEmpleadosStr.c_str());
		Logger::debug("Parámetro parseado 'cantEmpleados': " + cantEmpleadosStr, me);
	} else {
		Logger::error("Argumentos insuficientes. Se espera \n"
					  "$ ./empleado <idEmpleado> <cantEmpleados>", me);
		return 1;
	}

	// Get shmem sólo con cantEmpleados según Fer

	while (true) {
		// Me encolo en una cola que no sé dónde está (bloq)
		// Leo 4 bytes del fifo del jefe (bloq)
		// Pido paso al semáforo de surtidores (bloq)
		// Busco en la tabla un surtidor libre, y lo marco como ocupado
		// Cargo la nafta (delay random) (bloq)
		// Pido paso al semáforo de la caja registradora (bloq)
		// Sumo la cantidad total de dinero que me pagó el auto (delay random pequeño) (bloq)
		// Marco en la grilla que ya estoy libre
		break;
	}



	return 0;
}
