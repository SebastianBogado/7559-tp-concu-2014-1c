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

int main(int argc, char* argv[]) {
	const std::string me = __FILE__ ":main";
	
	// Parse command line
	Parser parser;
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

	// Ret val para los fork
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		Logger::log("Creando proceso JefeEstacion", Logger::LOG_NOTICE, me);
		// Creamos el nuevo proceso pasando el flag de debug, la cant de empleados y surtidores y un ID de proceso null
		// que no se utiliza en este caso
		ArgHelper argh;
		argh.encode("jefeEstacion", parser.debugMode(), parser.cantEmpleados(), parser.cantSurtidores(), 0);
		execv("./jefeEstacion", argh.getArgv());
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
			execv("./empleado", argh.getArgv());
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
	Logger::destroy();

	return 0;
}
