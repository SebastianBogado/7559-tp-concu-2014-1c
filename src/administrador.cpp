/*
 * administrador.cpp
 *
 *  Created on: May 10, 2014
 *      Author: ferno
 *
 *  Este proceso se encargará de verificar el valor de la caja registradora cada cierto tiempo random
 */

#include <cstdlib>
#include "common.h"
#include "comm/CajaRegistradora.h"
#include "comm/Surtidores.h"
#include "signal/SignalHandler.h"
#include "sighandlers/sigint.h"
#include "comm/ArgHelper.h"

int main(int argc, char* argv[]) {

	// Parsear los argumentos que nos manda el padre
	bool debugMode;
	int cantEmpleados, cantSurtidores, idEmpleado;
	
	if(!ArgHelper::decode(argc, argv, &debugMode, &cantEmpleados, &cantSurtidores, &idEmpleado)) {
		std::cerr << "Error - invalid parameters passed to process " << argv[0] << std::endl;
		exit(1);
	}

	// Init Logger
	if(debugMode) {
		Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	}else{
		Logger::initialize(logFile.c_str(), Logger::LOG_WARNING);
	}

	std::string procName = __FILE__":main";
	std::ostringstream logMsg("Logger initialized...");
	Logger::notice(logMsg.str(),procName);

	// Signal handlers
	SIGINT_Handler sigintHandler;
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigintHandler);
	
	// Main loop
	try {
		CajaRegistradora caja;
		try {
			caja.crearCaja();
		} catch(std::string& msg) {
			std::string _msg("Error en la creacion de la caja registradora");
			Logger::error(_msg, procName);
			exit(1);
		}
		
		logMsg.str("");
		logMsg << "Caja Registradora creada y lista para ser consultada";
		Logger::debug(logMsg.str(), procName);

		int sleepTime = 0;
		double monto = 0;
		while(!sigintHandler.gate()) {
			sleepTime = rand() % 10;
			logMsg.str("");
			logMsg << "El administrador esperará " << sleepTime << " segundos para consultar la caja";
			Logger::debug(logMsg.str(), procName);

			sleep(sleepTime);
			if(!sigintHandler.gate()) {
				sigintHandler.block();
				monto = caja.consultarMonto();

				logMsg.str("");
				logMsg << "En la caja hay actualmente " << monto << " pesos";
				Logger::error(logMsg.str(), procName);
				sigintHandler.unblock();
			}
		}
	}catch(std::string& err) {
		Logger::error("Excepcion: " + err, procName);
	}

	SignalHandler::destruir();
	Logger::debug("Fin del administrador", procName);
	Logger::destroy();
	return 0;
}
