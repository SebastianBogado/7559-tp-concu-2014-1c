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

int main(int argc, char* argv[]) {

	std::string procName = "Administrador";
	std::ostringstream logMsg("Logger initialized...");

	// TODO: Parser de los argumentos (ejemplo, ¿modo debug?)

	Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	Logger::notice(logMsg.str(),procName);

	// TODO: Manejo de señales para que termine el proceso!

	CajaRegistradora caja;

	logMsg.str("");logMsg.clear();
	logMsg << "Caja Registradora creada y lista para ser consultada";
	Logger::debug(logMsg.str(), procName);

	int sleepTime = 0;
	double monto = 0;
	for(;;) {
		sleepTime = rand() % 100;

		logMsg.str("");logMsg.clear();
		logMsg << "El administrador esperará " << sleepTime << " segundos para consultar la caja";
		Logger::debug(logMsg.str(), procName);

		sleep(sleepTime);
		monto = caja.consultarMonto();

		logMsg.str("");logMsg.clear();
		logMsg << "En la caja hay actualmente " << monto << " pesos";
		Logger::notice(logMsg.str(), procName);
		break;
	}

	CajaRegistradora::destruirCaja();
	return 0;
}
