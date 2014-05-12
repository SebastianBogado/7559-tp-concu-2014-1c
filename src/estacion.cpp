/*
*
*	TP 1 Técnicas de programación concurrentes I
*	Integrantes:
*		Bogado, Sebastián
*		Martty, Juan
*		Pereira, Fernando
*
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#include "logger/Logger.h"
#include "parser/Parser.h"
#include "common.h"
#include "comm/ArgHelper.h"

#include "comm/CajaRegistradora.h"
#include "comm/GrillaJefe.h"
#include "comm/Surtidores.h"
#include "comm/Fifo.h"

void inicializarSharedObjects(CajaRegistradora& caja, Grilla& grilla, GrillaJefe& grillaJefe, Surtidores& surtidores, unsigned int cantEmpleados, unsigned int cantSurtidores);

int main(int argc, char* argv[]) {
	const std::string me = __FILE__ ":main";
	
	// Parse command line
	Parser parser;
	std::string dir(argv[0]);
	dir.replace(dir.find("estacion"), dir.length() - dir.find("estacion"), "");
	if(!parser.parse(argc, argv)) {
		parser.printUsage();
		exit(2);
	}

	// Init Logger
	if(parser.debugMode()) {
		Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	}else{
		Logger::initialize(logFile.c_str(), Logger::LOG_WARNING);
	}
	
	Logger::debug("Se ha parseado la linea de comandos", me);

	// TODO: Ordenar la creacion de este fifo. Encapsularlo porque queda feito
	FifoEscritura fifoAutosJefe(fifoInputJefe);
	try {
		fifoAutosJefe.crear();
	} catch(std::string& msg) {
		std::string _msg("Error en al creacion del fifo entre input y jefe");
		Logger::error(_msg, me);
		exit(1);
	}
	CajaRegistradora caja;
	Grilla grilla;
	GrillaJefe grillaJefe(parser.cantEmpleados());
	Surtidores surtidores;

	try {
		inicializarSharedObjects(caja, grilla, grillaJefe, surtidores, parser.cantEmpleados(), parser.cantSurtidores());
	} catch(std::string& msg) {
		std::string _msg("Error en la inicializacion de objetos compartidos");
		Logger::error(_msg, me);
		exit(1);
	}
	// Ret val para los fork
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		Logger::log("Creando proceso JefeEstacion", Logger::LOG_NOTICE, me);
		// Creamos el nuevo proceso pasando el flag de debug, la cant de empleados y surtidores y un ID de proceso null
		// que no se utiliza en este caso
		ArgHelper argh;
		argh.encode("jefeEstacion", parser.debugMode(), parser.cantEmpleados(), parser.cantSurtidores(), 0);
		execv((dir + "jefeEstacion").c_str(), argh.getArgv());
		Logger::log("Error creando el hijo JefeEstacion", Logger::LOG_CRITICAL, me);
		Logger::destroy();
		exit(1);
	}

	// Fork & Execv Employees
	Logger::log("Creando procesos para empleados", Logger::LOG_CRITICAL, me);
	for(int i = 0;i < parser.cantEmpleados();i++) {
		pid = fork();
		if(pid == 0) {
			std::stringstream ss;
			ss << "Creando proceso empleado #" << i;
			Logger::log(ss.str(), Logger::LOG_NOTICE, me);
			// Creamos el nuevo proceso pasando el flag de debug, la cant de empleados y surtidores y el id
			ArgHelper argh;
			argh.encode("empleado", parser.debugMode(), parser.cantEmpleados(), parser.cantSurtidores(), i);
			execv((dir + "empleado").c_str() , argh.getArgv());
			ss.str("");
			ss << "Error creando proceso empleado #" << i;
			Logger::log(ss.str(), Logger::LOG_CRITICAL, me);
			Logger::destroy();
			exit(1);
		}
	}


	// Una vez creados los procesos, no hace mas nada
	Logger::debug("Waiting on children", me);
	for(;;) {
		int status;
		pid_t done = waitpid(-1, &status, 0);
		if(done == -1) {
			if (errno == ECHILD) {
				break; // no more child processes
			}else{
				std::cerr << "wtf?\n";
			}
		}else{
			if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
				std::stringstream ss;
				ss << "pid " << done << " failed";
				Logger::log(ss.str(), Logger::LOG_CRITICAL, me);
				Logger::destroy();
				exit(1);
			}
		}
	}

	Logger::debug("All children done", me);

	//fifoAutosJefe.eliminar();
	try {
	caja.destruirCaja();
	Logger::debug("La caja registradora se ha destruido por completo", me);
	grilla.destruir(parser.cantEmpleados());
	Logger::debug("La grilla se ha destruido por completo", me);
	grillaJefe.destruirGrillaJefe(parser.cantEmpleados());
	Logger::debug("La grilla del jefe se ha destruido por completo", me);
	surtidores.destruirSurtidores(parser.cantSurtidores());
	Logger::debug("Los surtidores se han destruido por completo", me);
	} catch(std::string& msg) {
		Logger::debug("Error en algunos de los ::destruir", me);
	}

	Logger::destroy();

	return 0;
}

void inicializarSharedObjects(CajaRegistradora& caja, Grilla& grilla, GrillaJefe& grillaJefe, Surtidores& surtidores,
		unsigned int cantEmpleados, unsigned int cantSurtidores) {
	caja.inicializarCaja();
	grilla.inicializarGrilla(cantEmpleados);
	grillaJefe.inicializarGrillaJefe();
	surtidores.inicializarSurtidores(cantSurtidores);
}
