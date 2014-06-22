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
#include "signal/SignalHandler.h"
#include "sighandlers/sigint.h"
#include "comm/CajaRegistradora.h"
#include "comm/GrillaJefe.h"
#include "comm/Surtidores.h"
#include "comm/ColaConPrioridad.hpp"

void inicializarSharedObjects(CajaRegistradora& caja, Grilla& grilla, GrillaJefe& grillaJefe, Surtidores& surtidores,
		unsigned int cantEmpleados, unsigned int cantSurtidores,
		std::ofstream& archivoColaInputJefe, ColaConPrioridad<automovil>& colaAutosJefe);

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

	// Signal handlers
	SIGINT_Handler sigintHandler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigintHandler);
	sigintHandler.block();

	CajaRegistradora caja;
	Grilla grilla;
	GrillaJefe grillaJefe(parser.cantEmpleados());
	Surtidores surtidores;
	std::ofstream archivoColaInputJefe(colaInputJefe.c_str());
	ColaConPrioridad<automovil> colaAutosJefe;


	try {
		inicializarSharedObjects(caja, grilla, grillaJefe, surtidores, parser.cantEmpleados(), parser.cantSurtidores(), archivoColaInputJefe, colaAutosJefe);
	} catch(std::string& msg) {
		Logger::error("Error en la inicializacion de objetos compartidos: " + msg, me);
		exit(1);
	}


	// Ret val para los fork
	pid_t pid, jefeEstacion_pid, administrador_pid;
	std::vector<pid_t> empleado_pid;
	
	// Jefe estacion
	jefeEstacion_pid = fork();
	if(jefeEstacion_pid == 0) {
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

	// Administrador
	administrador_pid = fork();
	if(administrador_pid == 0) {
		Logger::log("Creando proceso Administrador", Logger::LOG_NOTICE, me);
		// Creamos el nuevo proceso pasando el flag de debug, la cant de empleados y surtidores y un ID de proceso null
		// que no se utiliza en este caso
		ArgHelper argh;
		argh.encode("administrador", parser.debugMode(), parser.cantEmpleados(), parser.cantSurtidores(), 0);
		execv((dir + "administrador").c_str(), argh.getArgv());
		Logger::log("Error creando el hijo administrador", Logger::LOG_CRITICAL, me);
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
		}else{
			empleado_pid.push_back(pid);
		}
	}


	// Una vez creados los procesos, no hace mas nada
	Logger::debug("Waiting on children", me);
	for(;;) {
		int status;
		sigintHandler.unblock();
		pid_t done = waitpid(-1, &status, 0);
		sigintHandler.block();
		if(done == -1) {
			if(errno == ECHILD){
				break; // no more child processes
			}else if(errno == EINTR) {
				std::cerr << "-----------------INTERRUPTED!\n";
				kill(administrador_pid, SIGTERM);
				kill(jefeEstacion_pid, SIGTERM);
				for(size_t i = 0;i < empleado_pid.size();i++) {
					kill(empleado_pid[i], SIGTERM);
				}
				break;
			}else{
				std::cerr << "wtf?\n";
			}
		}else{
			if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
				std::stringstream ss;
				ss << "Proceso con pid  " << done << " died abnormally";
				Logger::log(ss.str(), Logger::LOG_CRITICAL, me);
				Logger::destroy();
				exit(1);
			}
			// Chequeamos si termino el jefe de la estacion para avisarle al administrador via signal
			if(done == jefeEstacion_pid) {
				Logger::notice("proceso jefe estacion termino", me);
				kill(administrador_pid, SIGINT);
			}
		}
	}

	Logger::debug("All children done", me);
	
	try {
		colaAutosJefe.destruir();
		caja.destruirCaja();
		Logger::debug("La caja registradora se ha destruido por completo", me);
		grilla.destruir(parser.cantEmpleados());
		Logger::debug("La grilla se ha destruido por completo", me);
		grillaJefe.destruirGrillaJefe(parser.cantEmpleados());
		Logger::debug("La grilla del jefe se ha destruido por completo", me);
		surtidores.destruirSurtidores(parser.cantSurtidores());
		Logger::debug("Los surtidores se han destruido por completo", me);
	} catch(std::string& msg) {
		Logger::debug("Error en algunos de los ::destruir: " + msg, me);
	}

	Logger::destroy();

	return 0;
}

void inicializarSharedObjects(CajaRegistradora& caja, Grilla& grilla, GrillaJefe& grillaJefe, Surtidores& surtidores,
		unsigned int cantEmpleados, unsigned int cantSurtidores,
		std::ofstream& archivoColaInputJefe, ColaConPrioridad<automovil>& colaAutosJefe) {
	caja.inicializarCaja();
	grilla.inicializarGrilla(cantEmpleados);
	grillaJefe.inicializarGrillaJefe();
	surtidores.inicializarSurtidores(cantSurtidores);

	if (archivoColaInputJefe.fail() || archivoColaInputJefe.bad()) {
		std::string me = __FILE__ ":inicializarSharedObjects";
		std::string _msg = std::string("Error creando archivo para la cola entre el jefe y el input: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}
	// TODO: Ordenar la creacion de esta cola. Encapsularlo porque queda feito
	colaAutosJefe = ColaConPrioridad<automovil> ( colaInputJefe, colaInputJefeKey );

}
