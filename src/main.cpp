/*
*
*	TP 1 Técnicas de programación concurrentes I
*	Integrantes:
*		Bogado, Sebastián
*		Martty, Juan
*		Pereira, Fernando
*
*/

#include "logger/Logger.h"

const char* logFile = "log.txt";

int main(int argc, char* argv[]) {

	// Init Logger
	Logger::initialize(logFile,Logger::LOG_DEBUG);

	// Parse command line (--cant-surtidores --cant-empleados ¿--log-filename?)
	Logger::log("Se ha parseado la linea de comandos",Logger::LOG_DEBUG);

	// Fork&Exev jefeEstacion
	Logger::log("Se ha creado el hijo Jefe De Estacion",Logger::LOG_NOTICE);

	// Fork&Execv Employees
	Logger::log("Se han creado los child process",Logger::LOG_CRITICAL);

	Logger::destroy();

	return 0;
}
