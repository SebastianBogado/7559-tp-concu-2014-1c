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
	Logger::initialize(logFile,Logger::LOG_DEBUG);

	// Parse command line (--cant-surtidores --cant-empleados ¿--log-filename?)
	Logger::log("Se ha parseado la linea de comandos", Logger::LOG_DEBUG);

  // Ret val para los fork
  pid_t pid;
  
	// Fork & Execv jefeEstacion
  pid = fork();
  if(pid == 0) {
    Logger::log("Creando proceso JefeEstacion", Logger::LOG_NOTICE);
    execv("./jefeEstacion", argv);
    Logger::log("Error creando el hijo JefeEstacion", Logger::LOG_CRITICAL);
    exit(1);
  }

	// Fork & Execv Employees
	Logger::log("Creando procesos para empleados", Logger::LOG_CRITICAL);

	Logger::destroy();

	return 0;
}