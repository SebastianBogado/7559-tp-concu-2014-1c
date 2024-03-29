#include <string>
#include <iostream>
#include <cstdlib>

#include "common.h"
#include "logger/Logger.h"
#include "comm/ColaConPrioridad.hpp"
#include "parser/Parser.h"
#include "comm/ArgHelper.h"
#include "comm/GrillaJefe.h"

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

	GrillaJefe grillaJefe(cantEmpleados);
	try {
		grillaJefe.crearGrillaJefe();
	} catch(std::string& msg) {
		std::string _msg("Error al crear la grilla del jefe");
		Logger::error(_msg, me);
		//TODO: Destrucciones
		exit(1);
	}

	// Setup de la cola de input
	ColaConPrioridad<automovil> colaAutosJefe = ColaConPrioridad<automovil>( colaInputJefe, colaInputJefeKey );
	bool leyendo = true;
	while(leyendo) {
		automovil automovil = colaAutosJefe.leer();
		if (automovil.id == automovilFinal.id) {
				Logger::debug("Sali del input loop por EOF", me);
				leyendo = false;
//		}else if(bytes == -1) {
//				Logger::debug("Sali del input loop por error de lectura", me);
//				leyendo = false;
//		}else if(bytes == -2) {
//				Logger::debug("Sali del input loop for interrupcion", me);
//				leyendo = false;
		} else {
			std::stringstream ss;
			ss << "Leí de la cola: " << automovil.id;
			if (automovil.esVip) ss << " (VIP)";
			Logger::debug(ss.str(), me);
			int for_empleado_id = grillaJefe.getEmpleadoLibre();
			if(for_empleado_id == -1) {
				Logger::debug("No hay empleados disponibles para este auto", me);
			}else{
				grillaJefe.asignarTrabajo(automovil.id, for_empleado_id);
				ss.str("");
				ss << "Se asigno el auto " << automovil.id << (automovil.esVip ? " (VIP)" : "") << " al empleado " << for_empleado_id;
				Logger::debug(ss.str(), me);
			}
		}
		sleep(5);
	}
	
	grillaJefe.avisarTerminarTrabajo(cantEmpleados);

}
