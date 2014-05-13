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

int main(int argc, char* argv[]) {

	std::string procName = "Administrador";
	std::ostringstream logMsg("Logger initialized...");

	// TODO: Parser de los argumentos (ejemplo, ¿modo debug?)

	Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	Logger::notice(logMsg.str(),procName);

	// TODO: Manejo de señales para que termine el proceso!
	try {
		CajaRegistradora caja;
		caja.crearCaja();
		
		logMsg.str("");logMsg.clear();
		logMsg << "Caja Registradora creada y lista para ser consultada";
		Logger::debug(logMsg.str(), procName);

		int sleepTime = 0;
		double monto = 0;
		for(;;) {
			sleepTime = rand() % 10;

			logMsg.str("");logMsg.clear();
			logMsg << "El administrador esperará " << sleepTime << " segundos para consultar la caja";
			Logger::debug(logMsg.str(), procName);

			sleep(sleepTime);
			monto = caja.consultarMonto();

			logMsg.str("");logMsg.clear();
			logMsg << "En la caja hay actualmente " << monto << " pesos";
			Logger::notice(logMsg.str(), procName);
		}
	}catch(std::string& err) {
		Logger::error("Excepcion: " + err, procName);
	}

	// TODO: La caja se debe destruir en el proceso padre (el que la crea!)
	//CajaRegistradora::destruirCaja();
	Logger::destroy();
	return 0;
}
