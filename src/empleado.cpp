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
#include <ctime>

#include "logger/Logger.h"
#include "common.h"
#include "comm/Surtidores.h"
#include "comm/CajaRegistradora.h"
#include "comm/GrillaEmpleado.h"
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
	Logger::initialize(logFile.c_str(), debugMode ? Logger::LOG_DEBUG : Logger::LOG_WARNING);

	srand(idEmpleado);

	std::string idEmpleadoStr = toString(idEmpleado),
		me = __FILE__"." + idEmpleadoStr + ":main";

	Logger::debug("Parámetros parseados: "
			"debug mode: " + toString(debugMode) + ", "
			"ID empleado: " + idEmpleadoStr + ", "
			"# empleados: " + toString(cantEmpleados) + ", "
			"# surtidores: " + toString(cantSurtidores), me);

	// Get shmem sólo con cantEmpleados según Fer

	CajaRegistradora cajaRegistradora;
	GrillaEmpleado grillaEmpleado(cantEmpleados);
	Surtidores surtidores(cantSurtidores);

	unsigned int idAuto, idSurtidor;
	double monto;
	std::string idAutoStr, idSurtidorStr, montoStr;

	while (true) {
		Logger::debug("Entro a la cola de empleados libres", me);
		// Me encolo en una cola que no sé dónde está (bloq)
		Logger::debug("Soy el próximo en la cola, el próximo auto me toca a mí", me);
		// Leo 4 bytes del fifo del jefe (bloq)
		idAuto = grillaEmpleado.esperarTrabajo(idEmpleado);
		// idAuto = rand() % 10000;
		idAutoStr = toString(idAuto);
		Logger::debug("El jefe me asignó el auto: " + idAutoStr + ". Voy a buscar surtidor libre", me);
		// Pido paso al semáforo de surtidores (bloq)
		// Busco en la tabla un surtidor libre, y lo marco como ocupado
		idSurtidor = surtidores.conseguirSurtidorLibre(idEmpleado);
		// idSurtidor = rand() % 10;
		idSurtidorStr = toString(idSurtidor);
		Logger::debug("Está libre el surtidor: " + idSurtidorStr + ". Cargando al auto: " + idAutoStr + "...", me);
		// Cargo la nafta (delay random) (bloq)
		sleep(rand() % 10 + 2);
		// Dejo de usar el surtidor y lo informo
		surtidores.liberarSurtidor(idSurtidor);
		Logger::debug("Terminé de cargarle al auto: " + idAutoStr + ". Libero el surtidor: " + idSurtidorStr, me);
		monto = (rand() % 20000 + 5000) / 100.0;
		montoStr = toString(monto);
		Logger::debug("Voy a la caja registradora a depositar: " + montoStr, me);
		// Pido paso al semáforo de la caja registradora (bloq)
		// Sumo la cantidad total de dinero que me pagó el auto (delay random pequeño) (bloq)
		cajaRegistradora.depositar(monto);
		Logger::debug("Deposité en la caja registradora: " + montoStr + " y vuelvo a ponerme libre", me);
		// Marco en la grilla que ya estoy libre
		grillaEmpleado.avisarTrabajoTerminado(idEmpleado);
	}


	Logger::destroy();

	return 0;
}
