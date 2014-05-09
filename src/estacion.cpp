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
#include "parser/Parser.h"
#include "common.h"

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
		execv("./jefeEstacion", argv);
		Logger::log("Error creando el hijo JefeEstacion", Logger::LOG_CRITICAL, me);
		exit(1);
	}

	// Fork & Execv Employees
	Logger::log("Creando procesos para empleados", Logger::LOG_CRITICAL, me);

	Logger::destroy();
	
	wait();

	return 0;
}
